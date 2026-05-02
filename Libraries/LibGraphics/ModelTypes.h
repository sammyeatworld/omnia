/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <vector>

#include <LibMath/Math.h>

namespace Graphics {

struct Vertex {
    Math::Vec3f position;
    Math::Vec2f tex_coord;
    Math::Vec3f normal;
};

using Index = u32;

struct SubMesh {
    std::vector<Vertex> vertices;
    std::vector<Index> indices;
};

struct ModelConfiguration {
    std::vector<SubMesh> sub_meshes;
};

}
