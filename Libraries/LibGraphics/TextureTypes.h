/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <vector>

#include <Common/Types.h>

namespace Graphics {

enum class TextureUsage : u8 {
    Sampled = 0,
    RenderTarget,
    DepthStencil
};

enum class TextureFormat : u8 {
    Unknown = 0,
    B8G8R8A8_SRGB,
    R8G8B8A8_SRGB,
    B8G8R8A8_UNORM,
    R8G8B8A8_UNORM,
    D32_SFLOAT,
};

struct TextureConfiguration {
    u32 width {};
    u32 height {};
    TextureFormat format {};
    TextureUsage usage = TextureUsage::Sampled;
    std::vector<u8> data;
};

}
