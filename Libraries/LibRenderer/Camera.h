/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <Common/Noncopyable.h>
#include <Common/Types.h>
#include <LibMath/Math.h>
#include <LibRenderer/Export.h>

namespace Renderer {

enum class ProjectionType {
    Perspective = 0,
    Orthographic
};

class RENDERER_API Camera final {
    OA_MAKE_DEFAULT_CONSTRUCTIBLE(Camera);

public:
    struct PerspectiveConfiguration {
        f32 aspect_ratio {};
        f32 field_of_view_degrees {};
        f32 near_plane {};
        f32 far_plane {};
    };

    struct OrthographicConfiguration {
    };

    struct Configuration {
        ProjectionType projection_type {};
        Math::Vec3f position {};
        union {
            PerspectiveConfiguration perspective;
            OrthographicConfiguration orthographic;
        };
    };

    Camera(Configuration const& config);

    auto projection() const -> Math::Mat4f;
    auto view() const -> Math::Mat4f;

    auto forward() const -> Math::Vec3f;
    auto right() const -> Math::Vec3f;
    auto up() const -> Math::Vec3f;

    void rotate(f32 pitch_degrees, f32 yaw_degrees, f32 roll_degrees);
    void translate(Math::Vec3f const& translation);
    void set_aspect_ratio(f32 aspect_ratio);
private:
    Configuration m_config {};
    Math::Quatf m_orientation;
    Math::Mat4f m_projection;
    Math::Mat4f m_view;
};

}
