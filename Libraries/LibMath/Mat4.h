/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <array>
#include <cassert>
#include <cmath>

#include <Common/Types.h>
#include <LibMath/Vec3.h>
#include <LibMath/Quat.h>

namespace Math {

template<typename T = f32>
class Mat4 {
public:
    constexpr Mat4() = default;

    constexpr Mat4(T diagonal)
    {
        m_elements[0] = diagonal;
        m_elements[5] = diagonal;
        m_elements[10] = diagonal;
        m_elements[15] = diagonal;
    }

    constexpr Mat4(std::array<T, 16> elements)
        : m_elements(elements)
    {
    }

    constexpr auto data() -> T*
    {
        return m_elements.data();
    }

    constexpr auto operator[](size_t index) -> T&
    {
        assert(index >= 0 && index < m_elements.size());
        return m_elements[index];
    }

    constexpr auto operator[](size_t index) const -> T const&
    {
        assert(index >= 0 && index < m_elements.size());
        return m_elements[index];
    }

    constexpr auto translate(Vec3<T> const& translation) const -> Mat4
    {
        Mat4 result = *this;
        result[12] += m_elements[0] * translation.x + m_elements[4] * translation.y + m_elements[8] * translation.z;
        result[13] += m_elements[1] * translation.x + m_elements[5] * translation.y + m_elements[9] * translation.z;
        result[14] += m_elements[2] * translation.x + m_elements[6] * translation.y + m_elements[10] * translation.z;
        return result;
    }

    static constexpr auto identity() -> Mat4
    {
        return Mat4(1);
    }

    static constexpr auto perspective(T fov, T aspect_ratio, T near_plane, T far_plane) -> Mat4
    {
        auto const tan_half_fov = std::tan(fov / static_cast<T>(2));
        auto const t = tan_half_fov * near_plane;
        auto const b = -t;
        auto const r = t * aspect_ratio;
        auto const l = -r;

        Mat4 result {};
        result[0] = (2 * near_plane) / (r - l);
        result[5] = -((2 * near_plane) / (t - b));
        result[10] = -far_plane / (far_plane - near_plane);
        result[11] = -1;
        result[14] = -(far_plane * near_plane) / (far_plane - near_plane);
        result[15] = 0;
        return result;
    }

    static constexpr auto from_quaternion(Quat<T> const& quat) -> Mat4
    {
        auto const xx = quat.x * quat.x;
        auto const yy = quat.y * quat.y;
        auto const zz = quat.z * quat.z;
        auto const xy = quat.x * quat.y;
        auto const xz = quat.x * quat.z;
        auto const yz = quat.y * quat.z;
        auto const wx = quat.w * quat.x;
        auto const wy = quat.w * quat.y;
        auto const wz = quat.w * quat.z;

        Mat4 result {};
        result[0] = 1 - (2 * (yy + zz));
        result[1] = 2 * (xy - wz);
        result[2] = 2 * (xz + wy);
        result[4] = 2 * (xy + wz);
        result[5] = 1 - (2 * (xx + zz));
        result[6] = 2 * (yz - wx);
        result[8] = 2 * (xz - wy);
        result[9] = 2 * (yz + wx);
        result[10] = 1 - (2 * (xx + yy));
        result[15] = 1;
        return result;
    }
private:
    std::array<T, 16> m_elements {};
};

using Mat4f = Mat4<f32>;
using Mat4d = Mat4<f64>;
using Mat4i = Mat4<i32>;
using Mat4l = Mat4<i64>;
using Mat4u = Mat4<u32>;
using Mat4ul = Mat4<u64>;

}
