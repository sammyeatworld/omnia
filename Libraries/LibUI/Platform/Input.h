/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <Common/Noncopyable.h>
#include <Common/Types.h>
#include <LibMath/Vec2.h>
#include <LibUI/Export.h>

#include <array>
#include <memory>
#include <vector>

namespace UI {

class UI_API Input final {
    OA_MAKE_NONCOPYABLE(Input);
    OA_MAKE_NONMOVABLE(Input);
    OA_MAKE_DEFAULT_CONSTRUCTIBLE(Input);

public:
    enum class Key : u8 {
        BackSpace = 0x08,
        Tab,
        Enter = 0x0D,

        Shift = 0x10,
        Control,
        Alt,

        CapsLock = 0x14,
        Escape = 0x1B,
        Space = 0x20,

        PageUp,
        PageDown,
        End,
        Home,
        LeftArrow,
        UpArrow,
        RightArrow,
        DownArrow,

        PrntScr = 0x2C,
        Insert,
        Delete,

        Num0 = 0x30,
        Num1,
        Num2,
        Num3,
        Num4,
        Num5,
        Num6,
        Num7,
        Num8,
        Num9,

        A = 0x41,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,

        NumPad0 = 0x60,
        NumPad1,
        NumPad2,
        NumPad3,
        NumPad4,
        NumPad5,
        NumPad6,
        NumPad7,
        NumPad8,
        NumPad9,

        F1 = 0x70,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,

        NumLock = 0x90,
        ScrollLock,

        Total
    };

    enum class MouseButton : u8 {
        Left,
        Right,
        Middle,
        Total
    };

    class Listener {
        OA_MAKE_NONCOPYABLE(Listener);
        OA_MAKE_NONMOVABLE(Listener);
        OA_MAKE_DEFAULT_CONSTRUCTIBLE(Listener);

    public:
        virtual ~Listener() = default;

        virtual auto on_key_pressed(Key /*unused*/) -> bool { return false; }
        virtual auto on_key_released(Key /*unused*/) -> bool { return false; }

        virtual auto on_mouse_button_pressed(MouseButton /*unused*/, Math::Vec2i const& /*unused*/) -> bool { return false; }
        virtual auto on_mouse_button_released(MouseButton /*unused*/, Math::Vec2i const& /*unused*/) -> bool { return false; }

        virtual auto on_mouse_move(Math::Vec2i const& /*unused*/) -> bool { return false; }
        virtual auto on_mouse_delta(Math::Vec2i const& /*unused*/) -> bool { return false; }
    };

    void add_listener(Listener* listener);
    void remove_listener(Listener* listener);

    void handle_key(Key key, bool pressed, bool was_pressed);
    void handle_mouse_button(MouseButton button, bool pressed, Math::Vec2i const& position = {});
    void handle_mouse_move(Math::Vec2i const& position);
    void handle_mouse_delta(Math::Vec2i const& delta);

    auto is_key_down(Key key) const -> bool;
    auto is_mouse_button_down(MouseButton button) const -> bool;
    auto mouse_position() const -> Math::Vec2i const&;

private:
    std::vector<Listener*> m_listeners;
    Math::Vec2i m_mouse_position {};
    std::array<bool, static_cast<std::size_t>(Key::Total)> m_keys {};
    std::array<bool, static_cast<std::size_t>(MouseButton::Total)> m_mouse_buttons {};
};

}
