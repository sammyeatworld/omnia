/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <LibAsset/ImportManager.h>

namespace Asset {

void ImportManager::register_default_importers()
{
    register_importer(std::make_shared<ShaderImporter>());
    register_importer(std::make_shared<TextureImporter>());
    register_importer(std::make_shared<ModelImporter>());
}

void ImportManager::register_importer(std::shared_ptr<Importer> importer)
{
    for (auto const& extension : importer->supported_extensions()) {
        m_importers[extension] = importer;
    }
}

}
