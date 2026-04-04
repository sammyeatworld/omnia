/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <functional>
#include <typeindex>
#include <variant>
#include <vector>

#include <LibMath/Vec2.h>
#include <LibUI/Export.h>
#include <LibUI/Platform/Input.h>

namespace UI {

struct KeyEvent {
    Input::Key key {};
    bool is_pressed;
};

struct MouseButtonEvent {
    Input::MouseButton button {};
    bool is_pressed;
    Math::Vec2i position {};
};

struct MouseMoveEvent {
    Math::Vec2i position {};
};

struct MouseDeltaEvent {
    i32 dx {};
    i32 dy {};
};

struct WindowResizeEvent {
    i32 width {};
    i32 height {};
};

struct WindowCloseEvent { };

using EventVariant = std::variant<
    KeyEvent,
    MouseButtonEvent,
    MouseMoveEvent,
    MouseDeltaEvent,
    WindowResizeEvent,
    WindowCloseEvent>;

class UI_API EventDispatcher {
public:
    template<typename EventType>
    using Callback = std::function<bool(EventType const&)>;

    template<typename EventType>
    static void register_listener(Callback<EventType> cb)
    {
        auto index = std::type_index(typeid(EventType));
        m_listeners[index].push_back([cb](EventVariant const& event) -> bool {
            return cb(std::get<EventType>(event));
        });
    }

    static void dispatch(EventVariant const& event)
    {
        auto index = std::visit([](auto const& e) -> std::type_index {
            return std::type_index(typeid(e));
        },
            event);

        auto it = m_listeners.find(index);
        if (it == m_listeners.end()) {
            return;
        }

        for (auto const& cb : m_listeners[index]) {
            if (cb(event)) {
                break;
            }
        }
    }
private:
    static std::unordered_map<std::type_index, std::vector<std::function<bool(EventVariant const&)>>> m_listeners;
};

}
