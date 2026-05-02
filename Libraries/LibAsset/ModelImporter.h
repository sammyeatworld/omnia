/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <LibAsset/Importer.h>
#include <LibGraphics/ModelTypes.h>

namespace Asset {

class ModelImporter final : public Importer {
public:
    auto import(std::filesystem::path const& path) -> std::expected<std::any, std::string> override;
    auto supported_extensions() const -> std::vector<std::string> override;
private:
    auto import_obj(std::filesystem::path const& path) -> std::expected<std::any, std::string>;
};

}
