/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <Common/Noncopyable.h>
#include <Common/Types.h>

namespace RHI {

class Swapchain {
    OA_MAKE_NONCOPYABLE(Swapchain);
    OA_MAKE_NONMOVABLE(Swapchain);

public:
    struct Configuration {
        i32 width;
        i32 height;
    };

    virtual ~Swapchain() = default;

    virtual void present() = 0;
    virtual auto config() const -> Configuration const& = 0;
protected:
    Swapchain() = default;
};

}
