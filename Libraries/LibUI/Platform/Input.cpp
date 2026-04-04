/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <LibUI/Platform/Input.h>
#include <LibUi/Platform/Event.h>

namespace UI {

void Input::handle_key(Key key, bool pressed, bool was_pressed)
{
    auto index = static_cast<std::size_t>(key);
    m_keys.at(index) = pressed;
    m_last_keys.at(index) = was_pressed;

    if (m_last_keys.at(index) != m_keys.at(index)) {
        KeyEvent event {
            .key = key,
            .is_pressed = pressed,
        };
        EventDispatcher::dispatch(event);
    }
}

void Input::handle_mouse_button(MouseButton button, bool pressed, Math::Vec2i const& position)
{
    auto index = static_cast<std::size_t>(button);
    m_last_mouse_buttons.at(index) = m_mouse_buttons.at(index);
    m_mouse_buttons.at(index) = pressed;

    if (m_last_mouse_buttons.at(index) != m_mouse_buttons.at(index)) {
        MouseButtonEvent event {
            .button = button,
            .is_pressed = pressed,
            .position = position
        };
        EventDispatcher::dispatch(event);
    }
}

void Input::handle_mouse_move(Math::Vec2i const& position)
{
    m_mouse_position = position;

    MouseMoveEvent event {
        .position = position
    };
    EventDispatcher::dispatch(event);
}

void Input::handle_mouse_delta(i32 dx, i32 dy)
{
    MouseDeltaEvent event {
        .dx = dx,
        .dy = dy
    };
    EventDispatcher::dispatch(event);
}

auto Input::is_key_down(Key key) const -> bool
{
    auto index = static_cast<std::size_t>(key);
    return m_keys.at(index);
}

auto Input::is_mouse_button_down(MouseButton button) const -> bool
{
    auto index = static_cast<std::size_t>(button);
    return m_mouse_buttons.at(index);
}

auto Input::mouse_position() const -> Math::Vec2i const&
{
    return m_mouse_position;
}

}
