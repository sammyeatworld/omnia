/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "Win32Window.h"

#include <Windows.h>
#include <bit>

namespace Core {

auto Window::create(Core::Window::Configuration const& config) -> std::expected<std::unique_ptr<Window>, std::string>
{
    return Win32Window::create(config);
}

auto Win32Window::create(Configuration const& config) -> std::expected<std::unique_ptr<Win32Window>, std::string>
{
    std::unique_ptr<Win32Window> window(new Win32Window);
    window->m_instance = GetModuleHandle(nullptr);
    window->m_config = config;

    WNDCLASSEX const window_class {
        .cbSize = sizeof(WNDCLASSEX),
        .style = 0,
        .lpfnWndProc = window_procedure,
        .cbClsExtra = 0,
        .cbWndExtra = 0,
        .hInstance = window->m_instance,
        .hIcon = nullptr,
        .hCursor = LoadCursor(nullptr, IDC_ARROW),
        .hbrBackground = nullptr,
        .lpszMenuName = nullptr,
        .lpszClassName = "omnia_window",
        .hIconSm = nullptr
    };
    if (!RegisterClassEx(&window_class)) {
        return std::unexpected("Failed to register window class.");
    }

    DWORD constexpr style = WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX | WS_SYSMENU;
    RECT rect { 0, 0, window->m_config.width, window->m_config.height };
    AdjustWindowRect(&rect, style, FALSE);

    window->m_handle = CreateWindowEx(
        0,
        window_class.lpszClassName,
        window->m_config.title.c_str(),
        style,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rect.right - rect.left, rect.bottom - rect.top,
        nullptr, nullptr, window->m_instance, window.get());
    if (window->m_handle == nullptr) {
        return std::unexpected("Failed to create window.");
    }
    ShowWindow(window->m_handle, SW_SHOW);

    return window;
}

Win32Window::~Win32Window()
{
    DestroyWindow(m_handle);
    UnregisterClass("omnia_window", m_instance);
}

auto Win32Window::window_procedure(HWND window_handle, u32 message, u64 first_param, i64 second_param) -> i64
{
    Win32Window* window {};

    if (message == WM_NCCREATE) {
        auto* create_struct = std::bit_cast<CREATESTRUCT*>(second_param);
        window = static_cast<Win32Window*>(create_struct->lpCreateParams);
        SetWindowLongPtr(window_handle, GWLP_USERDATA, std::bit_cast<LONG_PTR>(window));
    } else {
        window = std::bit_cast<Win32Window*>(GetWindowLongPtr(window_handle, GWLP_USERDATA));
    }

    if (window != nullptr && window->handle_message(message, first_param, second_param)) {
        return 0;
    }

    return DefWindowProc(window_handle, message, first_param, second_param);
}

auto Win32Window::handle_message(u32 message, [[maybe_unused]] u64 first_param, [[maybe_unused]] i64 second_param) -> bool
{
    switch (message) {
    case WM_CLOSE:
        m_is_running = false;
        return true;
    case WM_DESTROY:
        PostQuitMessage(0);
        return true;
    default:
        return false;
    }
}

void Win32Window::poll_events()
{
    MSG msg {};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

auto Win32Window::is_running() const -> bool
{
    return m_is_running;
}

auto Win32Window::title() const -> std::string const&
{
    return m_config.title;
}

auto Win32Window::width() const -> i32
{
    return m_config.width;
}

auto Win32Window::height() const -> i32
{
    return m_config.height;
}

}
