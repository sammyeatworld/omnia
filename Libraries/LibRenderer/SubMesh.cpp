/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <LibRenderer/SubMesh.h>

namespace Renderer {

auto SubMesh::create(Graphics::SubMeshConfiguration const& configuration, RHI::Device const* device) -> std::expected<SubMesh, std::string>
{
    RHI::Buffer::Configuration vertex_buffer_config {
        .size = configuration.vertices.size() * sizeof(Graphics::Vertex),
        .usage = RHI::BufferUsage::Vertex,
        .data = configuration.vertices.data()
    };
    auto vertex_buffer = device->create_buffer(vertex_buffer_config);
    if (!vertex_buffer) {
        return std::unexpected(std::move(vertex_buffer).error());
    }

    RHI::Buffer::Configuration index_buffer_config {
        .size = configuration.indices.size() * sizeof(Graphics::Index),
        .usage = RHI::BufferUsage::Index,
        .data = configuration.indices.data()
    };
    auto index_buffer = device->create_buffer(index_buffer_config);
    if (!index_buffer) {
        return std::unexpected(std::move(index_buffer).error());
    }

    SubMesh submesh;
    submesh.m_vertex_buffer = std::move(vertex_buffer).value();
    submesh.m_index_buffer = std::move(index_buffer).value();
    submesh.m_index_count = configuration.indices.size();
    submesh.m_material_index = configuration.material_index;
    return submesh;
}

auto SubMesh::vertex_buffer() const -> RHI::Buffer const*
{
    return m_vertex_buffer.get();
}

auto SubMesh::index_buffer() const -> RHI::Buffer const*
{
    return m_index_buffer.get();
}

auto SubMesh::index_count() const -> u64
{
    return m_index_count;
}

auto SubMesh::material_index() const -> u64
{
    return m_material_index;
}

}
