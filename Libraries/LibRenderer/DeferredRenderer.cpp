/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <Common/Expected.h>

#include <LibRenderer/DeferredRenderer.h>

namespace Renderer {

auto DeferredRenderer::create(Configuration const& config) -> std::expected<std::unique_ptr<DeferredRenderer>, std::string>
{
    assert(config.device);
    assert(config.resource_manager);

    std::unique_ptr<DeferredRenderer> renderer(new DeferredRenderer);
    renderer->m_device = config.device;

    return renderer->create_passes(config)
        .and_then([&]() {
            return renderer->create_resources(config);
        })
        .and_then([&]() {
            return renderer->create_pipelines(config);
        })
        .transform([&]() {
            return std::move(renderer);
        });
}

void DeferredRenderer::submit(SubmitInfo const& submit_info) const
{
    auto* cmd = submit_info.command_buffer;
    auto const* render_target = submit_info.output_render_target;

    m_frame_uniform_buffer->set_data(&submit_info.frame_data, sizeof(submit_info.frame_data));

    // Geometry pass
    cmd->begin_render_pass(m_geometry_render_pass.get(), m_gbuffer.render_target.get());
    {
        cmd->bind_pipeline(m_geometry_pipeline.get());
        cmd->set_viewport(0, 0, m_gbuffer.albedo->width(), m_gbuffer.albedo->height());
        cmd->set_scissor(0, 0, m_gbuffer.albedo->width(), m_gbuffer.albedo->height());
        cmd->bind_resource_set(0, m_frame_resource_set.get());

        for (auto const& render_item : submit_info.render_items) {
            cmd->bind_resource_set(1, render_item.material_resource_set);
            cmd->bind_vertex_buffer(render_item.vertex_buffer);
            cmd->bind_index_buffer(render_item.index_buffer);
            cmd->push_constants(m_model_push_constant, &render_item.model_matrix);
            cmd->draw_indexed(render_item.index_count, 1, 0, 0, 0);
        }
    }
    cmd->end_render_pass();

    // Lighting pass
    cmd->begin_render_pass(m_lighting_render_pass.get(), render_target);
    {
        cmd->bind_pipeline(m_lighting_pipeline.get());
        cmd->set_viewport(0, 0, render_target->width(), render_target->height());
        cmd->set_scissor(0, 0, render_target->width(), render_target->height());
        cmd->bind_resource_set(0, m_frame_resource_set.get());
        cmd->bind_resource_set(1, m_lighting_resource_set.get());
        cmd->draw(3, 1, 0, 0);
    }
    cmd->end_render_pass();
}

auto DeferredRenderer::resize(u32 width, u32 height) -> std::expected<void, std::string>
{
    auto gbuffer_result = create_gbuffer_textures(width, height);
    if (!gbuffer_result.has_value()) {
        return std::unexpected(std::move(gbuffer_result).error());
    }
    m_gbuffer = std::move(gbuffer_result).value();

    m_lighting_resource_set->set_texture(0, m_gbuffer.normal.get());
    m_lighting_resource_set->set_texture(1, m_gbuffer.albedo.get());
    m_lighting_resource_set->set_texture(2, m_gbuffer.material.get());
    m_lighting_resource_set->set_texture(3, m_gbuffer.emissive.get());
    m_lighting_resource_set->set_depth_texture(4, m_gbuffer.depth.get());

    return {};
}

auto DeferredRenderer::create_gbuffer_textures(u32 width, u32 height) -> std::expected<GBuffer, std::string>
{
    GBuffer gbuffer {};
    RHI::Texture::Configuration gbuffer_texture_config {
        .width = width,
        .height = height,
        .format = RHI::TextureFormat::R16G16B16A16_SFLOAT,
        .usage = RHI::TextureUsage::ColorAttachment | RHI::TextureUsage::Sampled,
        .data = {}
    };
    TRY_ASSIGN(gbuffer.normal, m_device->create_texture(gbuffer_texture_config));

    gbuffer_texture_config.format = RHI::TextureFormat::R8G8B8A8_UNORM;
    TRY_ASSIGN(gbuffer.albedo, m_device->create_texture(gbuffer_texture_config));
    TRY_ASSIGN(gbuffer.material, m_device->create_texture(gbuffer_texture_config));
    TRY_ASSIGN(gbuffer.emissive, m_device->create_texture(gbuffer_texture_config));

    RHI::Texture::Configuration const depth_texture_config {
        .width = width,
        .height = height,
        .format = RHI::TextureFormat::D32_SFLOAT,
        .usage = RHI::TextureUsage::DepthStencil | RHI::TextureUsage::Sampled,
        .data = {}
    };
    TRY_ASSIGN(gbuffer.depth, m_device->create_texture(depth_texture_config));

    RHI::RenderTarget::Configuration const geometry_render_target_config {
        .render_pass = m_geometry_render_pass.get(),
        .textures = {
            gbuffer.normal.get(),
            gbuffer.albedo.get(),
            gbuffer.material.get(),
            gbuffer.emissive.get()
        },
        .depth_texture = gbuffer.depth.get(),
        .width = width,
        .height = height
    };
    TRY_ASSIGN(gbuffer.render_target, m_device->create_render_target(geometry_render_target_config));

    return gbuffer;
}

auto DeferredRenderer::create_resources(Configuration const& config) -> std::expected<void, std::string>
{
    // Shaders
    TRY_ASSIGN(m_geometry_vertex_shader, config.resource_manager->load_shader("Shaders/GeometryPass.vs"));
    TRY_ASSIGN(m_geometry_fragment_shader, config.resource_manager->load_shader("Shaders/GeometryPass.fs"));
    TRY_ASSIGN(m_lighting_vertex_shader, config.resource_manager->load_shader("Shaders/LightingPass.vs"));
    TRY_ASSIGN(m_lighting_fragment_shader, config.resource_manager->load_shader("Shaders/LightingPass.fs"));

    auto gbuffer_result = create_gbuffer_textures(config.render_target_width, config.render_target_height);
    if (!gbuffer_result.has_value()) {
        return std::unexpected(std::move(gbuffer_result).error());
    }
    m_gbuffer = std::move(gbuffer_result).value();

    // Resource sets & layouts
    RHI::ResourceLayout::Configuration const gbuffer_layout_config {
        .bindings = {
            { .binding = 0, .type = RHI::ResourceType::Texture, .stage = Graphics::ShaderStage::Fragment },
            { .binding = 1, .type = RHI::ResourceType::Texture, .stage = Graphics::ShaderStage::Fragment },
            { .binding = 2, .type = RHI::ResourceType::Texture, .stage = Graphics::ShaderStage::Fragment },
            { .binding = 3, .type = RHI::ResourceType::Texture, .stage = Graphics::ShaderStage::Fragment },
            { .binding = 4, .type = RHI::ResourceType::Texture, .stage = Graphics::ShaderStage::Fragment }
        }
    };
    TRY_ASSIGN(m_lighting_resource_layout, config.device->create_resource_layout(gbuffer_layout_config));
    TRY_ASSIGN(m_lighting_resource_set, config.device->create_resource_set({ .layout = m_lighting_resource_layout.get() }));
    m_lighting_resource_set->set_texture(0, m_gbuffer.normal.get());
    m_lighting_resource_set->set_texture(1, m_gbuffer.albedo.get());
    m_lighting_resource_set->set_texture(2, m_gbuffer.material.get());
    m_lighting_resource_set->set_texture(3, m_gbuffer.emissive.get());
    m_lighting_resource_set->set_depth_texture(4, m_gbuffer.depth.get());

    RHI::Buffer::Configuration const frame_uniform_buffer_config {
        .size = sizeof(FrameData),
        .usage = RHI::BufferUsage::Uniform
    };
    TRY_ASSIGN(m_frame_uniform_buffer, config.device->create_buffer(frame_uniform_buffer_config));

    RHI::ResourceLayout::Configuration const frame_resource_layout_config {
        .bindings = {
            {
                .binding = 0,
                .type = RHI::ResourceType::Sampler,
                .stage = Graphics::ShaderStage::Fragment
            },
            {
                .binding = 1,
                .type = RHI::ResourceType::UniformBuffer,
                .stage = Graphics::ShaderStage::Vertex | Graphics::ShaderStage::Fragment
            }
        }
    };
    TRY_ASSIGN(m_frame_resource_layout, config.device->create_resource_layout(frame_resource_layout_config));

    RHI::Sampler::Configuration const sampler_config {
        .mag_filter = RHI::Filter::Linear,
        .min_filter = RHI::Filter::Linear,
        .address_mode = {
            .u = RHI::AddressMode::Repeat,
            .v = RHI::AddressMode::Repeat,
            .w = RHI::AddressMode::Repeat
        }
    };
    TRY_ASSIGN(m_default_sampler, config.device->create_sampler(sampler_config));

    RHI::ResourceSet::Configuration const frame_resource_set_config {
        .layout = m_frame_resource_layout.get(),
    };
    TRY_ASSIGN(m_frame_resource_set, config.device->create_resource_set(frame_resource_set_config));
    m_frame_resource_set->set_sampler(0, m_default_sampler.get());
    m_frame_resource_set->set_uniform_buffer(1, m_frame_uniform_buffer.get());

    m_model_push_constant = {
        .size = sizeof(Math::Mat4f),
        .offset = 0,
        .stage = Graphics::ShaderStage::Vertex
    };

    return {};
}

auto DeferredRenderer::create_passes(Configuration const& config) -> std::expected<void, std::string>
{
    RHI::RenderPass::Configuration const geometry_render_pass_config {
        .color_attachments = {
            {
                .format = RHI::TextureFormat::R16G16B16A16_SFLOAT,
                .load_op = RHI::LoadOp::Clear,
                .store_op = RHI::StoreOp::Store,
                .clear_color = { 0.0F, 0.0F, 0.0F, 0.0F },
                .initial_layout = RHI::ImageLayout::Undefined,
                .final_layout = RHI::ImageLayout::ShaderReadOnly
            },
            {
                .format = RHI::TextureFormat::R8G8B8A8_UNORM,
                .load_op = RHI::LoadOp::Clear,
                .store_op = RHI::StoreOp::Store,
                .clear_color = { 0.0F, 0.0F, 0.0F, 0.0F },
                .initial_layout = RHI::ImageLayout::Undefined,
                .final_layout = RHI::ImageLayout::ShaderReadOnly
            },
            {
                .format = RHI::TextureFormat::R8G8B8A8_UNORM,
                .load_op = RHI::LoadOp::Clear,
                .store_op = RHI::StoreOp::Store,
                .clear_color = { 0.0F, 0.0F, 0.0F, 0.0F },
                .initial_layout = RHI::ImageLayout::Undefined,
                .final_layout = RHI::ImageLayout::ShaderReadOnly
            },
            {
                .format = RHI::TextureFormat::R8G8B8A8_UNORM,
                .load_op = RHI::LoadOp::Clear,
                .store_op = RHI::StoreOp::Store,
                .clear_color = { 0.0F, 0.0F, 0.0F, 0.0F },
                .initial_layout = RHI::ImageLayout::Undefined,
                .final_layout = RHI::ImageLayout::ShaderReadOnly
            }
        },
        .depth_attachment = RHI::RenderPass::Attachment {
            .format = RHI::TextureFormat::D32_SFLOAT,
            .load_op = RHI::LoadOp::Clear,
            .store_op = RHI::StoreOp::Store,
            .clear_color = { 1.0F, 0.0F, 0.0F, 0.0F },
            .initial_layout = RHI::ImageLayout::Undefined,
            .final_layout = RHI::ImageLayout::DepthReadOnly
        }
    };
    TRY_ASSIGN(m_geometry_render_pass, config.device->create_render_pass(geometry_render_pass_config));

    RHI::RenderPass::Configuration const lighting_render_pass {
        .color_attachments = {
            {
                .format = config.render_target_format,
                .load_op = RHI::LoadOp::Clear,
                .store_op = RHI::StoreOp::Store,
                .clear_color = { 0.0F, 0.0F, 0.0F, 1.0F },
                .initial_layout = RHI::ImageLayout::Undefined,
                .final_layout = RHI::ImageLayout::PresentSrc
            }
        },
        .depth_attachment = std::nullopt
    };
    TRY_ASSIGN(m_lighting_render_pass, config.device->create_render_pass(lighting_render_pass));
    return {};
}

auto DeferredRenderer::create_pipelines(Configuration const& config) -> std::expected<void, std::string>
{
    RHI::Pipeline::Configuration const geometry_pipeline_config {
        .vertex_shader = m_geometry_vertex_shader,
        .fragment_shader = m_geometry_fragment_shader,
        .rasterization = {
            .cull_mode = RHI::CullMode::Back,
            .front_face = RHI::FrontFace::CounterClockwise,
            .polygon_mode = RHI::PolygonMode::Fill },
        .depth = { .test_enable = true, .write_enable = true, .compare_op = RHI::CompareOp::Less },
        .render_pass = m_geometry_render_pass.get(),
        .vertex_binding = RHI::Pipeline::VertexBinding { .stride = sizeof(Graphics::Vertex), .attributes = { { .location = 0, .offset = offsetof(Graphics::Vertex, position), .format = RHI::AttributeFormat::Float32Vec3 }, { .location = 1, .offset = offsetof(Graphics::Vertex, tex_coord), .format = RHI::AttributeFormat::Float32Vec2 }, { .location = 2, .offset = offsetof(Graphics::Vertex, normal), .format = RHI::AttributeFormat::Float32Vec3 }, { .location = 3, .offset = offsetof(Graphics::Vertex, tangent), .format = RHI::AttributeFormat::Float32Vec4 } } },
        .color_blend_attachments = { {}, {}, {}, {} },
        .resource_layouts = { m_frame_resource_layout.get(), config.resource_manager->resource_layout() },
        .push_constants = { m_model_push_constant }
    };
    TRY_ASSIGN(m_geometry_pipeline, config.device->create_pipeline(geometry_pipeline_config));

    RHI::Pipeline::Configuration const lighting_pipeline_config {
        .vertex_shader = m_lighting_vertex_shader,
        .fragment_shader = m_lighting_fragment_shader,
        .rasterization = {
            .cull_mode = RHI::CullMode::None,
            .front_face = RHI::FrontFace::CounterClockwise,
            .polygon_mode = RHI::PolygonMode::Fill },
        .depth = { .test_enable = false, .write_enable = false },
        .render_pass = m_lighting_render_pass.get(),
        .color_blend_attachments = { {} },
        .resource_layouts = { m_frame_resource_layout.get(), m_lighting_resource_layout.get() }
    };
    TRY_ASSIGN(m_lighting_pipeline, config.device->create_pipeline(lighting_pipeline_config));

    return {};
}

auto DeferredRenderer::create_output_render_target(RHI::Texture const* output_texture) const -> std::expected<std::unique_ptr<RHI::RenderTarget>, std::string>
{
    RHI::RenderTarget::Configuration const render_target_config {
        .render_pass = m_lighting_render_pass.get(),
        .textures = { output_texture },
        .depth_texture = nullptr,
        .width = output_texture->width(),
        .height = output_texture->height()
    };
    return m_device->create_render_target(render_target_config);
}

}
