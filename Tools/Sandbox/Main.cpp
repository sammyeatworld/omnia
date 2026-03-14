/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <Common/Types.h>
#include <LibGraphics/Device.h>
#include <LibUI/Platform/Event.h>
#include <LibUI/Platform/Window.h>

#include <algorithm>
#include <print>

class Sandbox final {
public:
    static auto create() -> std::optional<std::unique_ptr<Sandbox>>
    {
        std::unique_ptr<Sandbox> sandbox(new Sandbox);

        UI::Window::Configuration const window_config {
            .title = "Omnia Sandbox",
            .width = 800,
            .height = 600
        };
        auto window = UI::Window::create(window_config);
        if (!window.has_value()) {
            std::println(stderr, "Failed to create window: {}.", window.error());
            return std::nullopt;
        }

        sandbox->m_window = std::move(window.value());

        auto graphics_device = Graphics::Device::create(Graphics::Renderer::API::Vulkan);
        if (!graphics_device.has_value()) {
            std::println(stderr, "Failed to create graphics device: {}.", graphics_device.error());
            return std::nullopt;
        }
        sandbox->m_graphics_device = std::move(graphics_device.value());

        auto swapchain = sandbox->m_graphics_device->create_swapchain(sandbox->m_window.get(), {});
        if (!swapchain.has_value()) {
            std::println(stderr, "Failed to create swapchain: {}.", swapchain.error());
            return std::nullopt;
        }
        sandbox->m_swapchain = std::move(swapchain.value());
        sandbox->m_renderer = std::make_unique<Graphics::Renderer>(sandbox->m_graphics_device.get(), sandbox->m_swapchain.get());

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

            m_renderer->begin_frame();
            m_renderer->end_frame();

            m_swapchain->present();
        }
    }
private:
    Sandbox() = default;
private:
    std::unique_ptr<UI::Window> m_window {};
    std::unique_ptr<Graphics::Device> m_graphics_device {};
    std::unique_ptr<Graphics::Swapchain> m_swapchain {};
    std::unique_ptr<Graphics::Renderer> m_renderer {};
};

auto main() -> i32
{
    auto sandbox = Sandbox::create();
    if (!sandbox.has_value()) {
        std::println(stderr, "Failed to create sandbox application.");
        return EXIT_FAILURE;
    }
    sandbox.value()->run();

    return EXIT_SUCCESS;
}
