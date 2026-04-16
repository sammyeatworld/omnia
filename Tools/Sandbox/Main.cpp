/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <Common/Types.h>
#include <LibRHI/Device.h>
#include <LibUI/Platform/Event.h>
#include <LibUI/Platform/Window.h>

#include <algorithm>
#include <print>

class Sandbox final {
public:
    static auto create() -> std::expected<std::unique_ptr<Sandbox>, std::string>
    {
        std::unique_ptr<Sandbox> sandbox(new Sandbox);

        UI::Window::Configuration const window_config {
            .title = "Omnia Sandbox",
            .width = 800,
            .height = 600
        };
        auto window = UI::Window::create(window_config);
        if (!window.has_value()) {
            return std::unexpected(std::move(window.error()));
        }
        sandbox->m_window = std::move(window.value());

        RHI::Device::Configuration const device_config {
            .api = RHI::Device::API::Vulkan,
            .enable_debug_layer = true,
            .window = sandbox->m_window.get(),
        };
        auto graphics_device = RHI::Device::create(device_config);
        if (!graphics_device.has_value()) {
            return std::unexpected(std::move(graphics_device.error()));
        }
        sandbox->m_graphics_device = std::move(graphics_device.value());

        for (auto const& device_name : sandbox->m_graphics_device->physical_devices()) {
            if (sandbox->m_graphics_device->select_physical_device(device_name)) {
                std::println("Selected physical device: {}.", device_name);
                break;
            }
        }

        RHI::Swapchain::Configuration const swapchain_config {
            .width = window_config.width,
            .height = window_config.height,
            .frames_in_flight = 2
        };
        auto swapchain = sandbox->m_graphics_device->create_swapchain(swapchain_config);
        if (!swapchain.has_value()) {
            return std::unexpected(std::move(swapchain.error()));
        }
        sandbox->m_swapchain = std::move(swapchain.value());

        RHI::RenderPass::Configuration const main_render_pass_config {
            .color_attachments = {
                {
                    .format = sandbox->m_swapchain->format(),
                    .load_op = RHI::RenderPass::LoadOp::Clear,
                    .store_op = RHI::RenderPass::StoreOp::Store,
                    .clear_color = { 1.0F, 0.0F, 0.0F, 1.0F }
                }
            }
        };
        auto main_render_pass = sandbox->m_graphics_device->create_render_pass(main_render_pass_config);
        if (!main_render_pass.has_value()) {
            return std::unexpected(std::move(main_render_pass.error()));
        }
        sandbox->m_main_render_pass = std::move(main_render_pass.value());

        auto const& swapchain_textures = sandbox->m_swapchain->textures();
        for (auto const& swapchain_texture : swapchain_textures) {
            auto render_target = sandbox->m_graphics_device->create_render_target(sandbox->m_main_render_pass.get(), swapchain_texture.get());
            if (!render_target.has_value()) {
                return std::unexpected(std::move(render_target.error()));
            }
            sandbox->m_swapchain_render_targets.push_back(std::move(render_target.value()));
        }

        UI::EventDispatcher::register_listener<UI::KeyEvent>([](UI::KeyEvent const&) -> bool {
            std::println("Key event received!");
            return false;
        });
        UI::EventDispatcher::register_listener<UI::MouseButtonEvent>([](UI::MouseButtonEvent const&) -> bool {
            std::println("Mouse button event received!");
            return false;
        });
        UI::EventDispatcher::register_listener<UI::WindowCloseEvent>([](UI::WindowCloseEvent const&) -> bool {
            std::println("Window close event received!");
            return false;
        });
        UI::EventDispatcher::register_listener<UI::WindowResizeEvent>([](UI::WindowResizeEvent const& event) -> bool {
            std::println("Window resize event received! New size: {}x{}.", event.width, event.height);
            return false;
        });
        return sandbox;
    }

    void run()
    {
        while (m_window->is_running()) {
            m_window->poll_events();

            if (m_window->input().is_key_down(UI::Input::Key::F10)) {
                std::println("Input polling: F10 is down!");
            }

            if (m_window->input().is_mouse_button_down(UI::Input::MouseButton::Left)) {
                std::println("Input polling: Left mouse button is down!");
            }

            auto frame = m_swapchain->begin_frame();
            {
                frame.cmd->begin_render_pass(m_main_render_pass.get(), m_swapchain_render_targets[frame.image_index].get());
                {

                }
                frame.cmd->end_render_pass();
            }
            m_swapchain->end_frame(frame);
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
