/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <cmath>

#include <Common/Types.h>
#include <LibMath/Vec3.h>

namespace Math {

template<typename T = f32>
class Vec4 {
public:
    T x {};
    T y {};
    T z {};
    T w {};

    constexpr Vec4() = default;

    constexpr Vec4(T x, T y, T z, T w)
        : x(x)
        , y(y)
        , z(z)
        , w(w)
    {
    }

    constexpr auto xyz() const -> Vec3<T>
    {
        return Vec3<T> { x, y, z };
    }

    constexpr auto operator+(Vec4<T> const& other) const -> Vec4
    {
        return Vec4 {
            x + other.x,
            y + other.y,
            z + other.z,
            w + other.w
        };
    }

    constexpr auto operator-(Vec4<T> const& other) const -> Vec4
    {
        return Vec4 {
            x - other.x,
            y - other.y,
            z - other.z,
            w - other.w
        };
    }

    constexpr auto operator-() const -> Vec4
    {
        return Vec4 {
            -x,
            -y,
            -z,
            -w
        };
    }

    constexpr auto operator*(T scalar) const -> Vec4
    {
        return Vec4 {
            x * scalar,
            y * scalar,
            z * scalar,
            w * scalar
        };
    }

    constexpr auto operator/(T scalar) const -> Vec4
    {
        return Vec4 {
            x / scalar,
            y / scalar,
            z / scalar,
            w / scalar
        };
    }

    constexpr auto operator+=(Vec4<T> const& other) -> Vec4&
    {
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;
        return *this;
    }

    constexpr auto operator-=(Vec4<T> const& other) -> Vec4&
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        w -= other.w;
        return *this;
    }

    constexpr auto operator*=(T scalar) -> Vec4&
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        w *= scalar;
        return *this;
    }

    constexpr auto operator/=(T scalar) -> Vec4&
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        w /= scalar;
        return *this;
    }

    constexpr auto length() const -> T
    {
        return std::sqrt((x * x) + (y * y) + (z * z) + (w * w));
    }

    constexpr void normalize()
    {
        auto length = this->length();

        if (length != 0) {
            x /= length;
            y /= length;
            z /= length;
            w /= length;
        }
    }

    constexpr auto normalized() const -> Vec4
    {
        auto length = this->length();

        if (length != 0) {
            return Vec4 {
                x / length,
                y / length,
                z / length,
                w / length
            };
        }

        return *this;
    }
};

using Vec4f = Vec4<f32>;
using Vec4d = Vec4<f64>;
using Vec4i = Vec4<i32>;
using Vec4l = Vec4<i64>;
using Vec4u = Vec4<u32>;
using Vec4ul = Vec4<u64>;

}
