/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <LibAsset/AssetRegistry.h>
#include <LibAsset/Export.h>
#include <LibAsset/Importer.h>
#include <LibAsset/TextureImporter.h>
#include <LibGraphics/ModelTypes.h>

namespace Asset {

struct MaterialData {
    std::string name;
    std::optional<AssetID> albedo_texture_id = std::nullopt;
    Math::Vec4f base_color { 1.0f, 0.0f, 1.0f, 1.0f };
};

struct ModelData {
    std::vector<Graphics::SubMeshData> sub_meshes;
    std::vector<MaterialData> materials;
};

class ASSET_API ModelImporter final {
public:
    static auto import(std::filesystem::path const& path, AssetRegistry const& asset_registry) -> std::expected<ModelData, std::string>;
    static auto supported_extensions() -> std::vector<std::string>;
private:
    static auto import_gltf(std::filesystem::path const& path, AssetRegistry const& asset_registry) -> std::expected<ModelData, std::string>;
};

template<>
struct ImporterTrait<ModelData> {
    using type = ModelImporter;
};

}
