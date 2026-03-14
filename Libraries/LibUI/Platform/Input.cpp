/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "Input.h"

namespace UI {

void Input::add_listener(Listener* listener)
{
    m_listeners.emplace_back(listener);
}

void Input::remove_listener(Listener* listener)
{
    auto found = std::ranges::find(m_listeners.begin(), m_listeners.end(), listener);
    m_listeners.erase(found);
}

void Input::handle_key(Key key, bool pressed, bool was_pressed)
{
    auto index = static_cast<std::size_t>(key);
    m_keys.at(index) = pressed;

    for (auto* listener : m_listeners) {
        if (pressed && !was_pressed && listener->on_key_pressed(key)) {
            break;
        } else if (!pressed && was_pressed && listener->on_key_released(key)) {
            break;
        }
    }
}

void Input::handle_mouse_button(MouseButton button, bool pressed, Math::Vec2i const& position)
{
    auto index = static_cast<std::size_t>(button);
    m_mouse_buttons.at(index) = pressed;

    for (auto* listener : m_listeners) {
        if (pressed && listener->on_mouse_button_pressed(button, position)) {
            break;
        } else if (listener->on_mouse_button_released(button, position)) {
            break;
        }
    }
}

void Input::handle_mouse_move(Math::Vec2i const& position)
{
    m_mouse_position = position;

    for (auto* listener : m_listeners) {
        if (listener->on_mouse_move(position)) {
            break;
        }
    }
}

void Input::handle_mouse_delta(Math::Vec2i const& delta)
{
    for (auto* listener : m_listeners) {
        if (listener->on_mouse_delta(delta)) {
            break;
        }
    }
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