/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <cmath>

#include <Common/Types.h>

namespace Math {

template<typename T = f32>
class Vec3 {
public:
    T x {};
    T y {};
    T z {};

    constexpr Vec3() = default;

    constexpr Vec3(T x, T y, T z)
        : x(x)
        , y(y)
        , z(z)
    {
    }

    constexpr void normalize()
    {
        auto const length = std::sqrt(x * x + y * y + z * z);

        if (length != 0) {
            x /= length;
            y /= length;
            z /= length;
        }
    }

    constexpr auto operator+(Vec3<T> const& other) const -> Vec3
    {
        return Vec3 {
            x + other.x,
            y + other.y,
            z + other.z
        };
    }

    constexpr auto operator-(Vec3<T> const& other) const -> Vec3
    {
        return Vec3 {
            x - other.x,
            y - other.y,
            z - other.z
        };
    }

    constexpr auto operator-() const -> Vec3
    {
        return Vec3 {
            -x,
            -y,
            -z
        };
    }

    constexpr auto operator*(T scalar) const -> Vec3
    {
        return Vec3 {
            x * scalar,
            y * scalar,
            z * scalar
        };
    }

    constexpr auto operator+=(Vec3<T> const& other) -> Vec3&
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    static constexpr auto normalize(Vec3<T> vec) -> Vec3
    {
        vec.normalize();
        return vec;
    }

    static constexpr auto cross(Vec3<T> const& a, Vec3<T> const& b) -> Vec3
    {
        return Vec3 {
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        };
    }

    static constexpr auto dot(Vec3<T> const& a, Vec3<T> const& b) -> T
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }
};

using Vec3f = Vec3<f32>;
using Vec3d = Vec3<f64>;
using Vec3i = Vec3<i32>;
using Vec3l = Vec3<i64>;
using Vec3u = Vec3<u32>;
using Vec3ul = Vec3<u64>;

}
