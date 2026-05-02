/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <LibRenderer/Camera.h>

namespace Renderer {

Camera::Camera(Configuration const& config)
    : m_config(config)
{
    if (m_config.projection_type == ProjectionType::Perspective) {
        auto const& perspective = m_config.perspective;
        m_projection = Math::Mat4f::perspective(DEG_TO_RAD(perspective.field_of_view_degrees), perspective.aspect_ratio, perspective.near_plane, perspective.far_plane);
    } else {
    }
    m_view = Math::Mat4f::from_quaternion(m_orientation).translate(-m_config.position);
}

auto Camera::projection() const -> Math::Mat4f
{
    return m_projection;
}

auto Camera::view() const -> Math::Mat4f
{
    return m_view;
}

auto Camera::forward() const -> Math::Vec3f
{
    return m_orientation * Math::Vec3f(0, 0, -1);
}

auto Camera::right() const -> Math::Vec3f
{
    return m_orientation * Math::Vec3f(1, 0, 0);
}

auto Camera::up() const -> Math::Vec3f
{
    return m_orientation * Math::Vec3f(0, 1, 0);
}

void Camera::rotate(f32 pitch_degrees, f32 yaw_degrees, f32 roll_degrees)
{
    auto const pitch_quat = Math::Quatf::from_axis_angle(Math::Vec3f { 1.0F, 0.0F, 0.0F }, DEG_TO_RAD(pitch_degrees));
    auto const yaw_quat = Math::Quatf::from_axis_angle(Math::Vec3f { 0.0F, 1.0F, 0.0F }, DEG_TO_RAD(yaw_degrees));
    auto const roll_quat = Math::Quatf::from_axis_angle(Math::Vec3f { 0.0F, 0.0F, 1.0F }, DEG_TO_RAD(roll_degrees));

    m_orientation = (yaw_quat * m_orientation * pitch_quat * roll_quat).normalized();
    m_view = Math::Mat4f::from_quaternion(m_orientation).translate(-m_config.position);
}

void Camera::translate(Math::Vec3f const& translation)
{
    m_config.position += translation;
    m_view = Math::Mat4f::from_quaternion(m_orientation).translate(-m_config.position);
}

void Camera::set_aspect_ratio(f32 aspect_ratio)
{
    if (m_config.projection_type == ProjectionType::Perspective) {
        auto& perspective = m_config.perspective;
        perspective.aspect_ratio = aspect_ratio;
        m_projection = Math::Mat4f::perspective(DEG_TO_RAD(perspective.field_of_view_degrees), perspective.aspect_ratio, perspective.near_plane, perspective.far_plane);
    }
}

}
