/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <Common/Noncopyable.h>
#include <Common/Types.h>

namespace RHI {

class RenderTarget {
    OA_MAKE_NONCOPYABLE(RenderTarget);
    OA_MAKE_DEFAULT_MOVABLE(RenderTarget);

public:
    virtual ~RenderTarget() = default;
protected:
    RenderTarget() = default;
};

}
