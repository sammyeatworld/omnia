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
    struct Frame {
    };

    struct Configuration {
        i32 width;
        i32 height;
        i32 frames_in_flight = 2;
    };

    virtual ~Swapchain() = default;

    virtual auto begin_frame() -> Frame = 0;
    virtual void end_frame(Frame const& frame) = 0;
protected:
    Swapchain() = default;
};

}
