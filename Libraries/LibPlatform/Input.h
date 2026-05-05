/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <array>
#include <memory>

#include <Common/Noncopyable.h>
#include <Common/Types.h>
#include <LibMath/Vec2.h>
#include <LibPlatform/Export.h>
#include <LibPlatform/Event.h>
#include <LibPlatform/InputCodes.h>

namespace Platform {

class PLATFORM_API Input final {
    OA_MAKE_DEFAULT_CONSTRUCTIBLE(Input);

public:
    Input(EventDispatcher* dispatcher);

    void handle_key(Key key, bool pressed, bool was_pressed);
    void handle_mouse_button(MouseButton button, bool pressed, Math::Vec2i const& position = {});
    void handle_mouse_move(Math::Vec2i const& position);
    void handle_mouse_delta(i32 dx, i32 dy);

    auto is_key_down(Key key) const -> bool;
    auto is_mouse_button_down(MouseButton button) const -> bool;
    auto mouse_position() const -> Math::Vec2i const&;
private:
    EventDispatcher* m_dispatcher;
    Math::Vec2i m_mouse_position {};
    std::array<bool, static_cast<std::size_t>(Key::Total)> m_keys {};
    std::array<bool, static_cast<std::size_t>(Key::Total)> m_last_keys {};
    std::array<bool, static_cast<std::size_t>(MouseButton::Total)> m_mouse_buttons {};
    std::array<bool, static_cast<std::size_t>(MouseButton::Total)> m_last_mouse_buttons {};
};

}
