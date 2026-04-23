/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <Common/Noncopyable.h>
#include <Common/Types.h>
#include <LibMath/Vec3.h>

namespace RHI {

enum class Filter : u8 {
    Nearest = 0,
    Linear
};

enum class AddressMode : u8 {
    Repeat = 0,
    MirroredRepeat,
    ClampToEdge,
    ClampToBorder
};

class Sampler {
    OA_MAKE_NONCOPYABLE(Sampler);
    OA_MAKE_DEFAULT_MOVABLE(Sampler);

public:
    struct Configuration {
        Filter mag_filter;
        Filter min_filter;
        struct {
            AddressMode u;
            AddressMode v;
            AddressMode w;
        } address_mode;
    };

    virtual ~Sampler() = default;
protected:
    Sampler() = default;
};

}
