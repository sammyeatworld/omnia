/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <expected>
#include <string>

#include <LibGraphics/ModelTypes.h>
#include <LibRenderer/Export.h>
#include <LibRHI/Device.h>

namespace Renderer {

class RENDERER_API SubMesh final {
    OA_MAKE_NONCOPYABLE(SubMesh);
    OA_MAKE_DEFAULT_MOVABLE(SubMesh);

public:
    static auto create(Graphics::SubMeshConfiguration const& configuration, RHI::Device const* device) -> std::expected<SubMesh, std::string>;

    auto vertex_buffer() const -> RHI::Buffer const*;
    auto index_buffer() const -> RHI::Buffer const*;
    auto index_count() const -> u64;
    auto material_index() const -> u64;
private:
    SubMesh() = default;
private:
    std::unique_ptr<RHI::Buffer> m_vertex_buffer;
    std::unique_ptr<RHI::Buffer> m_index_buffer;
    u64 m_index_count;
    u64 m_material_index;
};

}
