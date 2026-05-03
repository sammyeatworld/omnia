/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <print>
#include <LibRenderer/Model.h>

namespace Renderer {

auto Model::create(Graphics::ModelConfiguration const& configuration, RHI::Device* device, RHI::ResourceLayout const* resource_layout) -> std::expected<std::unique_ptr<Model>, std::string>
{
    std::unique_ptr<Model> model(new Model);
    return model->create_sub_meshes(configuration.sub_meshes, device)
        .and_then([&]() {
            return model->create_materials(configuration.materials, device, resource_layout);
        })
        .transform([&]() {
            return std::move(model);
        });
}

auto Model::create_sub_meshes(std::vector<Graphics::SubMeshConfiguration> const& configurations, RHI::Device const* device) -> std::expected<void, std::string>
{
    for (auto const& sub_mesh_configuration : configurations) {
        auto sub_mesh = SubMesh::create(sub_mesh_configuration, device);
        if (!sub_mesh.has_value()) {
            return std::unexpected(sub_mesh.error());
        }
        m_sub_meshes.push_back(std::move(sub_mesh).value());
    }
    return {};
}

auto Model::create_materials(std::vector<Graphics::MaterialConfiguration> const& configurations, RHI::Device* device, RHI::ResourceLayout const* resource_layout) -> std::expected<void, std::string>
{
    for (auto const& material_configuration : configurations) {
        auto material = Material::create(material_configuration, device, resource_layout);
        if (!material.has_value()) {
            return std::unexpected(material.error());
        }
        m_materials.push_back(std::move(material).value());
    }
    return {};
}

auto Model::sub_meshes() const -> std::vector<SubMesh> const&
{
    return m_sub_meshes;
}

auto Model::materials() const -> std::vector<Material> const&
{
    return m_materials;
}

}
