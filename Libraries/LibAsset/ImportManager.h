/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <memory>
#include <unordered_map>

#include <LibAsset/Export.h>
#include <LibAsset/Importer.h>
#include <LibAsset/ModelImporter.h>
#include <LibAsset/ShaderImporter.h>
#include <LibAsset/TextureImporter.h>

namespace Asset {

class ASSET_API ImportManager final {
public:
    ImportManager() = default;

    void register_default_importers();
    void register_importer(std::shared_ptr<Importer> importer);

    template<typename T>
    auto import(std::filesystem::path const& path) -> std::expected<T, std::string>
    {
        auto extension = path.extension().string();
        auto it = m_importers.find(extension);
        if (it == m_importers.end()) {
            return std::unexpected(std::format("No importer registered for extension '{}'", extension));
        }

        auto import_result = it->second->import(path);
        if (!import_result.has_value()) {
            return std::unexpected(import_result.error());
        }

        if (auto* casted_data = std::any_cast<T>(&import_result.value())) {
            return *casted_data;
        } else {
            return std::unexpected(std::format("Importer for extension '{}' returned data of incorrect type", extension));
        }
    }
private:
    std::unordered_map<std::string, std::shared_ptr<Importer>> m_importers;
};

}
