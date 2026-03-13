/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include "Input.h"
#include <Common/Noncopyable.h>
#include <Common/Platform.h>
#include <Common/Types.h>
#include <LibCore/Export.h>

#include <expected>
#include <memory>
#include <string>

namespace Core {

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

    static auto CORE_API create(Configuration const& config) -> std::expected<std::unique_ptr<Window>, std::string>;
protected:
    Window() = default;
};

}
