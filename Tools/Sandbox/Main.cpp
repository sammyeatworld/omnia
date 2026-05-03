/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <Common/Types.h>
#include <LibAsset/ImportManager.h>
#include <LibMath/Math.h>
#include <LibRenderer/Camera.h>
#include <LibRHI/Device.h>
#include <LibUI/Platform/Event.h>
#include <LibUI/Platform/Window.h>

#include <print>

#define TRY_ASSIGN(lhs, expr)                                  \
    {                                                          \
        auto result = (expr);                                  \
        if (!result.has_value()) {                             \
            return std::unexpected(std::move(result.error())); \
        }                                                      \
        lhs = std::move(result.value());                       \
    }                                                          \
    (void)0

struct PerFrameData {
    Math::Mat4f projection;
    Math::Mat4f view;
};

// TODO: Cleanup all into LibRenderer
class Sandbox final {
public:
    static auto create() -> std::expected<std::unique_ptr<Sandbox>, std::string>
    {
        std::unique_ptr<Sandbox> sandbox(new Sandbox);
        sandbox->m_import_manager.register_default_importers();

        UI::Window::Configuration const window_config {
            .title = "Omnia Sandbox",
            .width = 800,
            .height = 600
        };
        TRY_ASSIGN(sandbox->m_window, UI::Window::create(window_config));

        RHI::Device::Configuration const device_config {
            .api = RHI::Device::API::Vulkan,
            .enable_debug_layer = true,
            .window = sandbox->m_window.get(),
        };
        TRY_ASSIGN(sandbox->m_graphics_device, RHI::Device::create(device_config));

        RHI::Swapchain::Configuration const swapchain_config {
            .width = static_cast<u32>(window_config.width),
            .height = static_cast<u32>(window_config.height),
            .frames_in_flight = 2
        };
        TRY_ASSIGN(sandbox->m_swapchain, sandbox->m_graphics_device->create_swapchain(swapchain_config));

        Graphics::ModelConfiguration cube_config;
        TRY_ASSIGN(sandbox->m_cube_config, sandbox->m_import_manager.import<Graphics::ModelConfiguration>("Resources/Models/viking_room.obj"));

        Renderer::Camera::Configuration const camera_config {
            .projection_type = Renderer::ProjectionType::Perspective,
            .position = { 0.0F, 0.0F, 0.0F },
            .perspective = {
                .aspect_ratio = static_cast<f32>(swapchain_config.width) / static_cast<f32>(swapchain_config.height),
                .field_of_view_degrees = 90.0F,
                .near_plane = 0.1F,
                .far_plane = 100.0F
            }
        };
        sandbox->m_camera = Renderer::Camera(camera_config);

        RHI::RenderPass::Configuration const main_render_pass_config {
            .color_attachments = {
                {
                    .format = sandbox->m_swapchain->format(),
                    .load_op = RHI::LoadOp::Clear,
                    .store_op = RHI::StoreOp::Store,
                    .clear_color = { 1.0F, 0.0F, 0.0F, 1.0F }
                }
            },
            .depth_attachment = RHI::RenderPass::Attachment {
                .format = Graphics::TextureFormat::D32_SFLOAT,
                .load_op = RHI::LoadOp::Clear,
                .store_op = RHI::StoreOp::DontCare,
                .clear_color = { 1.0F, 0.0F, 0.0F, 1.0F }
            }
        };
        TRY_ASSIGN(sandbox->m_main_render_pass, sandbox->m_graphics_device->create_render_pass(main_render_pass_config));

        RHI::Texture::Configuration const depth_texture_config {
            .width = swapchain_config.width,
            .height = swapchain_config.height,
            .format = Graphics::TextureFormat::D32_SFLOAT,
            .usage = Graphics::TextureUsage::DepthStencil,
            .data = {}
        };
        TRY_ASSIGN(sandbox->m_depth_texture, sandbox->m_graphics_device->create_texture(depth_texture_config));

        auto const& swapchain_textures = sandbox->m_swapchain->textures();
        for (auto const& swapchain_texture : swapchain_textures) {
            auto render_target = sandbox->m_graphics_device->create_render_target(sandbox->m_main_render_pass.get(), swapchain_texture.get(), sandbox->m_depth_texture.get());
            if (!render_target.has_value()) {
                return std::unexpected(std::move(render_target.error()));
            }
            sandbox->m_swapchain_render_targets.push_back(std::move(render_target.value()));
        }

        RHI::Buffer::Configuration const cube_vertex_buffer_config {
            .size = sizeof(Graphics::Vertex) * sandbox->m_cube_config.sub_meshes[0].vertices.size(),
            .usage = RHI::BufferUsage::Vertex,
            .data = sandbox->m_cube_config.sub_meshes[0].vertices.data()
        };
        TRY_ASSIGN(sandbox->m_cube_vb, sandbox->m_graphics_device->create_buffer(cube_vertex_buffer_config));

        RHI::Buffer::Configuration const cube_index_buffer_config {
            .size = sizeof(Graphics::Index) * sandbox->m_cube_config.sub_meshes[0].indices.size(),
            .usage = RHI::BufferUsage::Index,
            .data = sandbox->m_cube_config.sub_meshes[0].indices.data()
        };
        TRY_ASSIGN(sandbox->m_cube_ib, sandbox->m_graphics_device->create_buffer(cube_index_buffer_config));

        {
            RHI::Shader::Configuration shader_config;
            TRY_ASSIGN(shader_config, sandbox->m_import_manager.import<RHI::Shader::Configuration>("Resources/Shaders/BaseObject.fs.glsl"));
            TRY_ASSIGN(sandbox->m_fragment_shader, sandbox->m_graphics_device->create_shader(shader_config));
        }

        {
            RHI::Shader::Configuration shader_config;
            TRY_ASSIGN(shader_config, sandbox->m_import_manager.import<RHI::Shader::Configuration>("Resources/Shaders/BaseObject.vs.glsl"));
            TRY_ASSIGN(sandbox->m_vertex_shader, sandbox->m_graphics_device->create_shader(shader_config));
        }

        RHI::Texture::Configuration texture_config;
        TRY_ASSIGN(texture_config, sandbox->m_import_manager.import<RHI::Texture::Configuration>("Resources/Textures/viking_room.png"));
        TRY_ASSIGN(sandbox->m_texture, sandbox->m_graphics_device->create_texture(texture_config));

        RHI::Buffer::Configuration const uniform_buffer_config {
            .size = sizeof(PerFrameData),
            .usage = RHI::BufferUsage::Uniform
        };
        TRY_ASSIGN(sandbox->m_per_frame, sandbox->m_graphics_device->create_buffer(uniform_buffer_config));

        RHI::ResourceLayout::Configuration const main_resource_layout_config {
            .bindings = {
                {
                    .binding = 0,
                    .type = RHI::ResourceType::Texture,
                    .stage = Graphics::ShaderStage::Fragment
                },
                {
                    .binding = 1,
                    .type = RHI::ResourceType::Sampler,
                    .stage = Graphics::ShaderStage::Fragment
                },
                {
                    .binding = 2,
                    .type = RHI::ResourceType::UniformBuffer,
                    .stage = Graphics::ShaderStage::Vertex
                }
            }
        };
        TRY_ASSIGN(sandbox->m_main_resource_layout, sandbox->m_graphics_device->create_resource_layout(main_resource_layout_config));

        RHI::Sampler::Configuration const sampler_config {
            .mag_filter = RHI::Filter::Linear,
            .min_filter = RHI::Filter::Linear,
            .address_mode = {
                .u = RHI::AddressMode::Repeat,
                .v = RHI::AddressMode::Repeat,
                .w = RHI::AddressMode::Repeat
            }
        };
        TRY_ASSIGN(sandbox->m_sampler, sandbox->m_graphics_device->create_sampler(sampler_config));

        RHI::ResourceSet::Configuration const resource_set_config {
            .layout = sandbox->m_main_resource_layout.get(),
        };
        TRY_ASSIGN(sandbox->m_resource_set, sandbox->m_graphics_device->create_resource_set(resource_set_config));
        sandbox->m_resource_set->set_texture(0, sandbox->m_texture.get());
        sandbox->m_resource_set->set_sampler(1, sandbox->m_sampler.get());
        sandbox->m_resource_set->set_uniform_buffer(2, sandbox->m_per_frame.get());

        sandbox->m_per_object_push_constant = {
            .size = sizeof(Math::Mat4f),
            .offset = 0,
            .stage = Graphics::ShaderStage::Vertex
        };

        RHI::Pipeline::Configuration const main_pipeline_config {
            .vertex_shader = sandbox->m_vertex_shader.get(),
            .fragment_shader = sandbox->m_fragment_shader.get(),
            .rasterization = {
                .cull_mode = RHI::CullMode::Back,
                .front_face = RHI::FrontFace::CounterClockwise,
                .polygon_mode = RHI::PolygonMode::Fill
            },
            .depth = {
                .test_enable = true,
                .write_enable = true,
                .compare_op = RHI::CompareOp::Less
            },
            .render_pass = sandbox->m_main_render_pass.get(),
            .vertex_binding = {
                .stride = sizeof(Graphics::Vertex),
                .attributes = {
                    {
                        .location = 0,
                        .offset = offsetof(Graphics::Vertex, position),
                        .format = RHI::AttributeFormat::Float32Vec3
                    },
                    {
                        .location = 1,
                        .offset = offsetof(Graphics::Vertex, tex_coord),
                        .format = RHI::AttributeFormat::Float32Vec2
                    },
                    {
                        .location = 2,
                        .offset = offsetof(Graphics::Vertex, normal),
                        .format = RHI::AttributeFormat::Float32Vec3
                    }
                }
            },
            .resource_layouts = {
                sandbox->m_main_resource_layout.get()
            },
            .push_constants = {
                sandbox->m_per_object_push_constant
            }
        };
        TRY_ASSIGN(sandbox->m_pipeline, sandbox->m_graphics_device->create_pipeline(main_pipeline_config));

        sandbox->m_window->event_dispatcher().register_listener<UI::MouseDeltaEvent>(std::bind_front(&Sandbox::on_mouse_delta, sandbox.get()));
        sandbox->m_window->event_dispatcher().register_listener<UI::WindowResizeEvent>(std::bind_front(&Sandbox::on_resize, sandbox.get()));
        return sandbox;
    }

    auto on_mouse_delta(UI::MouseDeltaEvent const& event) -> bool
    {
        m_camera.rotate(static_cast<f32>(-event.dy) * 0.1F, static_cast<f32>(-event.dx) * 0.1F, 0.0F);
        return true;
    }

    auto on_resize([[maybe_unused]] UI::WindowResizeEvent const& event) -> bool
    {
        m_camera.set_aspect_ratio(static_cast<f32>(m_window->width()) / static_cast<f32>(m_window->height()));
        m_was_window_resized = true;
        return true;
    }

    auto recreate_swapchain() -> bool
    {
        m_swapchain->wait_idle();
        m_swapchain_render_targets.clear();
        m_depth_texture.reset();

        RHI::Swapchain::Configuration const new_swapchain_config {
            .width = static_cast<u32>(m_window->width()),
            .height = static_cast<u32>(m_window->height()),
            .frames_in_flight = 2
        };

        auto result = m_swapchain->recreate(new_swapchain_config);
        if (!result.has_value()) {
            std::println(stderr, "{}", result.error());
            return false;
        }

        RHI::Texture::Configuration const depth_texture_config {
            .width = new_swapchain_config.width,
            .height = new_swapchain_config.height,
            .format = Graphics::TextureFormat::D32_SFLOAT,
            .usage = Graphics::TextureUsage::DepthStencil,
            .data = {}
        };
        auto depth_texture_result = m_graphics_device->create_texture(depth_texture_config);
        if (!depth_texture_result.has_value()) {
            std::println(stderr, "{}", depth_texture_result.error());
            return false;
        }
        m_depth_texture = std::move(depth_texture_result.value());

        auto const& swapchain_textures = m_swapchain->textures();
        for (auto const& swapchain_texture : swapchain_textures) {
            auto render_target = m_graphics_device->create_render_target(m_main_render_pass.get(), swapchain_texture.get(), m_depth_texture.get());
            if (!render_target.has_value()) {
                std::println(stderr, "{}", render_target.error());
                return false;
            }
            m_swapchain_render_targets.push_back(std::move(render_target.value()));
        }
        return true;
    }

    void run()
    {
        while (m_window->is_running()) {
            m_window->poll_events();

            if (m_window->is_minimized()) {
                continue;
            }

            if (m_was_window_resized || m_swapchain->is_dirty()) {
                m_was_window_resized = false;
                if (!recreate_swapchain()) {
                    break;
                }
                continue;
            }

            auto frame = m_swapchain->begin_frame();
            if (!frame.has_value()) {
                continue;
            }

            auto const& input = m_window->input();

            if (input.is_key_down(UI::Key::W) || input.is_key_down(UI::Key::A) || input.is_key_down(UI::Key::S) || input.is_key_down(UI::Key::D)) {
                Math::Vec3f movement {};
                movement += m_camera.forward() * (input.is_key_down(UI::Key::W) ? 1.0F : 0.0F);
                movement -= m_camera.forward() * (input.is_key_down(UI::Key::S) ? 1.0F : 0.0F);
                movement -= m_camera.right() * (input.is_key_down(UI::Key::A) ? 1.0F : 0.0F);
                movement += m_camera.right() * (input.is_key_down(UI::Key::D) ? 1.0F : 0.0F);
                movement.normalize();
                m_camera.translate(movement * 0.01F);
            }

            PerFrameData const per_frame_data {
                .projection = m_camera.projection(),
                .view = m_camera.view()
            };
            m_per_frame->set_data(&per_frame_data, sizeof(PerFrameData));

            auto [cmd, image_index, frame_index] = frame.value();
            {
                cmd->begin_render_pass(m_main_render_pass.get(), m_swapchain_render_targets[image_index].get());
                {
                    cmd->bind_pipeline(m_pipeline.get());
                    {
                        cmd->set_viewport(0, 0, m_swapchain->width(), m_swapchain->height());
                        cmd->set_scissor(0, 0, m_swapchain->width(), m_swapchain->height());
                        cmd->bind_resource_set(0, m_resource_set.get());
                        cmd->bind_vertex_buffer(m_cube_vb.get());
                        cmd->bind_index_buffer(m_cube_ib.get());

                        auto model_matrix = Math::Mat4f::rotation(DEG_TO_RAD(90.0F), 0.0F, 0.0F);
                        cmd->push_constants(m_per_object_push_constant, &model_matrix);

                        cmd->draw_indexed(m_cube_config.sub_meshes[0].indices.size(), 1, 0, 0, 0);
                    }
                }
                cmd->end_render_pass();
            }
            m_swapchain->end_frame(frame.value());
        }
    }

    ~Sandbox()
    {
        if (m_swapchain != nullptr) {
            m_swapchain->wait_idle();
        }
    }
private:
    Sandbox() = default;
private:
    std::unique_ptr<UI::Window> m_window;
    std::unique_ptr<RHI::Device> m_graphics_device;
    std::unique_ptr<RHI::Swapchain> m_swapchain;
    std::unique_ptr<RHI::RenderPass> m_main_render_pass;
    std::vector<std::unique_ptr<RHI::RenderTarget>> m_swapchain_render_targets;
    std::unique_ptr<RHI::Pipeline> m_pipeline;
    Asset::ImportManager m_import_manager;
    std::unique_ptr<RHI::Shader> m_vertex_shader;
    std::unique_ptr<RHI::Shader> m_fragment_shader;
    std::unique_ptr<RHI::Buffer> m_cube_vb;
    std::unique_ptr<RHI::Buffer> m_cube_ib;
    std::unique_ptr<RHI::ResourceLayout> m_main_resource_layout;
    std::unique_ptr<RHI::ResourceSet> m_resource_set;
    std::unique_ptr<RHI::Texture> m_texture;
    std::unique_ptr<RHI::Texture> m_depth_texture;
    std::unique_ptr<RHI::Sampler> m_sampler;
    std::unique_ptr<RHI::Buffer> m_per_frame;
    RHI::Pipeline::PushConstant m_per_object_push_constant {};
    bool m_was_window_resized = false;
    Graphics::ModelConfiguration m_cube_config;
    Renderer::Camera m_camera {};
};

auto main() -> i32
{
    auto sandbox = Sandbox::create();
    if (!sandbox.has_value()) {
        std::println(stderr, "{}", sandbox.error());
        return EXIT_FAILURE;
    }
    sandbox.value()->run();

    return EXIT_SUCCESS;
}
