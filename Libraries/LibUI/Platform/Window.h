/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <expected>
#include <memory>
#include <string>

#include <Common/Noncopyable.h>
#include <Common/Types.h>
#include <LibUI/Export.h>
#include <LibUI/Platform/Input.h>

namespace UI {

class Window {
    OA_MAKE_NONCOPYABLE(Window);
    OA_MAKE_NONMOVABLE(Window);

public:
    struct Configuration {
        std::string title;
        i32 width;
        i32 height;
    };

    virtual ~Window() = default;

    virtual void poll_events() = 0;
    virtual auto input() -> Input& = 0;
    virtual auto is_running() const -> bool = 0;
    virtual auto title() const -> std::string const& = 0;
    virtual auto width() const -> i32 = 0;
    virtual auto height() const -> i32 = 0;

    static auto UI_API create(Configuration const& config) -> std::expected<std::unique_ptr<Window>, std::string>;
protected:
    Window() = default;
};

}
