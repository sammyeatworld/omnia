/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <vector>

#include <Common/Noncopyable.h>
#include <Common/Types.h>
#include <LibRHI/Forward.h>

namespace RHI {

enum class CullMode : u8 {
    None = 0,
    Front,
    Back
};

enum class FrontFace : u8 {
    Clockwise = 0,
    CounterClockwise
};

enum class PolygonMode : u8 {
    Fill = 0,
    Line,
    Point
};

enum class CompareOp : u8 {
    Never = 0,
    Less,
    Equal,
    LessOrEqual,
    Greater,
    NotEqual,
    GreaterOrEqual,
    Always
};

enum class AttributeFormat : u8 {
    Float32 = 0,
    Float32Vec2,
    Float32Vec3,
    Float32Vec4,
};

class Pipeline {
    OA_MAKE_NONCOPYABLE(Pipeline);
    OA_MAKE_DEFAULT_MOVABLE(Pipeline);

public:
    struct Rasterization {
        CullMode cull_mode;
        FrontFace front_face;
        PolygonMode polygon_mode;
    };

    struct Depth {
        bool test_enable;
        CompareOp compare_op;
    };

    struct VertexAttribute {
        u32 location;
        u32 offset;
        AttributeFormat format;
    };

    struct VertexBinding {
        u32 stride;
        std::vector<VertexAttribute> attributes;
    };

    struct Configuration {
        Shader const* vertex_shader {};
        Shader const* fragment_shader {};
        Rasterization rasterization {};
        Depth depth {};
        RenderPass const* render_pass {};
        VertexBinding vertex_binding {};
        std::vector<ResourceLayout const*> resource_layouts {};
    };

    virtual ~Pipeline() = default;
protected:
    Pipeline() = default;
};

}
