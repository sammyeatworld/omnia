/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <Common/Types.h>
#include <LibAsset/ImportManager.h>
#include <LibAsset/ShaderImporter.h>
#include <LibRHI/Device.h>
#include <LibUI/Platform/Event.h>
#include <LibUI/Platform/Window.h>

#include <algorithm>
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

class Sandbox final {
public:
    static auto create() -> std::expected<std::unique_ptr<Sandbox>, std::string>
    {
        std::unique_ptr<Sandbox> sandbox(new Sandbox);

        sandbox->m_import_manager.register_importer(std::make_shared<Asset::ShaderImporter>());

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

        RHI::RenderPass::Configuration const main_render_pass_config {
            .color_attachments = {
                {
                    .format = sandbox->m_swapchain->format(),
                    .load_op = RHI::LoadOp::Clear,
                    .store_op = RHI::StoreOp::Store,
                    .clear_color = { 1.0F, 0.0F, 0.0F, 1.0F }
                }
            }
        };
        TRY_ASSIGN(sandbox->m_main_render_pass, sandbox->m_graphics_device->create_render_pass(main_render_pass_config));

        auto const& swapchain_textures = sandbox->m_swapchain->textures();
        for (auto const& swapchain_texture : swapchain_textures) {
            auto render_target = sandbox->m_graphics_device->create_render_target(sandbox->m_main_render_pass.get(), swapchain_texture.get());
            if (!render_target.has_value()) {
                return std::unexpected(std::move(render_target.error()));
            }
            sandbox->m_swapchain_render_targets.push_back(std::move(render_target.value()));
        }

        struct Vertex {
            Math::Vec3f position;
        };

        std::vector<Vertex> const triangle_vertices {
            { .position = {  0.5F, -0.5F, 1.0F } },
            { .position = {  0.5F,  0.5F, 1.0F } },
            { .position = { -0.5F,  0.5F, 1.0F } },
            { .position = { -0.5F, -0.5F, 1.0F } }
        };

        RHI::Buffer::Configuration const triangle_vertex_buffer_config {
            .size = sizeof(Vertex) * triangle_vertices.size(),
            .usage = RHI::BufferUsage::Vertex,
            .data = triangle_vertices.data()
        };
        TRY_ASSIGN(sandbox->m_quad_vb, sandbox->m_graphics_device->create_buffer(triangle_vertex_buffer_config));

        std::vector<u32> const triangle_indices {
            0, 1, 2, 0, 2, 3
        };
        RHI::Buffer::Configuration const triangle_index_buffer_config {
            .size = sizeof(u32) * triangle_indices.size(),
            .usage = RHI::BufferUsage::Index,
            .data = triangle_indices.data()
        };
        TRY_ASSIGN(sandbox->m_quad_ib, sandbox->m_graphics_device->create_buffer(triangle_index_buffer_config));

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

        RHI::ResourceLayout::Configuration const main_resource_layout_config {
            .bindings = {
                {
                    .binding = 0,
                    .type = RHI::ResourceType::UniformBuffer,
                    .stage = RHI::ShaderStage::Vertex
                }
            }
        };
        TRY_ASSIGN(sandbox->m_main_resource_layout, sandbox->m_graphics_device->create_resource_layout(main_resource_layout_config));

        RHI::ResourceSet::Configuration const main_resource_set_config {
            .layout = sandbox->m_main_resource_layout.get(),
        };
        TRY_ASSIGN(sandbox->m_main_resource_set, sandbox->m_graphics_device->create_resource_set(main_resource_set_config));

        RHI::Pipeline::Configuration const main_pipeline_config {
            .vertex_shader = sandbox->m_vertex_shader.get(),
            .fragment_shader = sandbox->m_fragment_shader.get(),
            .rasterization = {
                .cull_mode = RHI::CullMode::None,
                .front_face = RHI::FrontFace::CounterClockwise,
                .polygon_mode = RHI::PolygonMode::Fill
            },
            .depth = {
                .test_enable = true,
                .compare_op = RHI::CompareOp::Less
            },
            .render_pass = sandbox->m_main_render_pass.get(),
            .vertex_binding = {
                .stride = sizeof(Vertex),
                .attributes = {
                    {
                        .location = 0,
                        .offset = offsetof(Vertex, position),
                        .format = RHI::AttributeFormat::Float32Vec3
                    }
                }
            },
            .resource_layouts = {
                sandbox->m_main_resource_layout.get()
            }
        };
        TRY_ASSIGN(sandbox->m_pipeline, sandbox->m_graphics_device->create_pipeline(main_pipeline_config));

        sandbox->m_window->event_dispatcher().register_listener<UI::WindowCloseEvent>([](UI::WindowCloseEvent const&) {
            std::println("Window close event received, exiting...");
            return true;
        });
        sandbox->m_window->event_dispatcher().register_listener<UI::WindowResizeEvent>(std::bind_front(&Sandbox::on_resize, sandbox.get()));
        return sandbox;
    }

    auto on_resize([[maybe_unused]] UI::WindowResizeEvent const& event) -> bool
    {
        m_was_window_resized = true;
        return true;
    }

    auto recreate_swapchain() -> bool
    {
        m_swapchain->wait_idle();
        m_swapchain_render_targets.clear();

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

        auto const& swapchain_textures = m_swapchain->textures();
        for (auto const& swapchain_texture : swapchain_textures) {
            auto render_target = m_graphics_device->create_render_target(m_main_render_pass.get(), swapchain_texture.get());
            if (!render_target.has_value()) {
                std::println(stderr, "{}", render_target.error());
                return false;
            }
            m_swapchain_render_targets.push_back(std::move(render_target.value()));
        }
        std::println("Recreating swapchain...");
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
            auto [cmd, image_index] = frame.value();
            {
                cmd->begin_render_pass(m_main_render_pass.get(), m_swapchain_render_targets[image_index].get());
                {
                    cmd->bind_pipeline(m_pipeline.get());
                    {
                        cmd->set_viewport(0, 0, m_swapchain->width(), m_swapchain->height());
                        cmd->set_scissor(0, 0, m_swapchain->width(), m_swapchain->height());
                        cmd->bind_resource_set(0, m_main_resource_set.get());
                        cmd->bind_vertex_buffer(m_quad_vb.get());
                        cmd->bind_index_buffer(m_quad_ib.get());
                        cmd->draw_indexed(6, 1, 0, 0, 0);
                    }
                }
                cmd->end_render_pass();
            }
            m_swapchain->end_frame(frame.value());
        }
    }

    ~Sandbox()
    {
        m_swapchain->wait_idle();
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
    std::unique_ptr<RHI::Buffer> m_quad_vb;
    std::unique_ptr<RHI::Buffer> m_quad_ib;
    std::unique_ptr<RHI::ResourceLayout> m_main_resource_layout;
    std::unique_ptr<RHI::ResourceSet> m_main_resource_set;
    bool m_was_window_resized = false;
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
