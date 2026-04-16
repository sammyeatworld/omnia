/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <Common/Types.h>

namespace Math {

template<typename T = f32>
class Vec4 {
public:
    T x {};
    T y {};
    T z {};
    T w {};

    Vec4() = default;

    Vec4(T x, T y, T z, T w)
        : x(x)
        , y(y)
        , z(z)
        , w(w)
    {
    }
};

using Vec4f = Vec4<f32>;
using Vec4d = Vec4<f64>;
using Vec4i = Vec4<i32>;
using Vec4l = Vec4<i64>;
using Vec4u = Vec4<u32>;
using Vec4ul = Vec4<u64>;

}
