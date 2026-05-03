/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <LibGraphics/ModelTypes.h>
#include <LibRHI/Device.h>
#include <LibRenderer/Export.h>

namespace Renderer {

class RENDERER_API Material final {
    OA_MAKE_NONCOPYABLE(Material);
    OA_MAKE_DEFAULT_MOVABLE(Material);

public:
    static auto create(Graphics::MaterialConfiguration const& configuration, RHI::Device* device, RHI::ResourceLayout const* resource_layout) -> std::expected<Material, std::string>;

    auto resource_set() const -> RHI::ResourceSet const*;
private:
    Material() = default;
private:
    std::unique_ptr<RHI::Texture> m_albedo_texture;
    std::unique_ptr<RHI::ResourceSet> m_resource_set;
};

}
