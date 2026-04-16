/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <Common/Noncopyable.h>
#include <LibMath/Vec3.h>

namespace RHI {

class Texture {
    OA_MAKE_NONCOPYABLE(Texture);
    OA_MAKE_DEFAULT_MOVABLE(Texture);

public:
    enum class Format : u8 {
        Unknown = 0,
        B8G8R8A8_SRGB,
        B8G8R8A8_UNORM
    };

    struct Configuration {
        u32 width {};
        u32 height {};
        Format format {};
    };

    virtual ~Texture() = default;

    virtual auto config() const -> Configuration const& = 0;
protected:
    Texture() = default;
};

}
