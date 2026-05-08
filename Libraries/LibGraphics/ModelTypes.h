/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <optional>
#include <string>
#include <vector>

#include <LibMath/Math.h>

namespace Graphics {

struct Vertex {
    Math::Vec3f position;
    Math::Vec2f tex_coord;
    Math::Vec3f normal;
    Math::Vec4f tangent;
};

using Index = u32;

struct MaterialParameters {
    Math::Vec4f base_color { 1.0F, 1.0F, 1.0F, 1.0F };
    Math::Vec3f emissive_factor { 0.0F, 0.0F, 0.0F };
    f32 metallic_factor = 1.0F;
    f32 roughness_factor = 1.0F;
    f32 normal_scale = 1.0F;
    f32 occlusion_strength = 1.0F;
};

struct SubMeshData {
    std::vector<Vertex> vertices;
    std::vector<Index> indices;
    u64 material_index;
};

}
