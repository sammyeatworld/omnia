/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <LibRenderer/Material.h>

namespace Renderer {

auto Material::create(Graphics::MaterialConfiguration const& configuration, RHI::Device* device, RHI::ResourceLayout const* resource_layout) -> std::expected<Material, std::string>
{
    Material material;

    if (!configuration.albedo_texture_configuration.has_value()) {
        RHI::Texture::Configuration default_texture_configuration {
            .width = 1,
            .height = 1,
            .format = Graphics::TextureFormat::R8G8B8A8_UNORM,
            .data = {}
        };
        default_texture_configuration.data.resize(4);
        std::memset(default_texture_configuration.data.data(), 255, 4);
        auto default_texture_result = device->create_texture(default_texture_configuration);
        if (!default_texture_result.has_value()) {
            return std::unexpected(std::move(default_texture_result).error());
        }
        material.m_albedo_texture = std::move(default_texture_result).value();
    } else {
        auto albedo_texture_result = device->create_texture(configuration.albedo_texture_configuration.value());
        if (!albedo_texture_result.has_value()) {
            return std::unexpected(std::move(albedo_texture_result).error());
        }
        material.m_albedo_texture = std::move(albedo_texture_result).value();
    }

    auto resource_set_configuration = RHI::ResourceSet::Configuration {
        .layout = resource_layout,
    };
    auto resource_set_result = device->create_resource_set(resource_set_configuration);
    if (!resource_set_result.has_value()) {
        return std::unexpected(std::move(resource_set_result).error());
    }
    material.m_resource_set = std::move(resource_set_result).value();

    material.m_resource_set->set_texture(0, material.m_albedo_texture.get());

    return material;
}

auto Material::resource_set() const -> RHI::ResourceSet const*
{
    return m_resource_set.get();
}

}
