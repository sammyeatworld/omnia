/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <vector>

#include <Common/Types.h>

namespace RHI {

enum class TextureFormat : u8 {
    Unknown = 0,
    B8G8R8A8_SRGB,
    R8G8B8A8_SRGB,
    B8G8R8A8_UNORM,
    R8G8B8A8_UNORM
};

struct TextureConfiguration {
    u32 width {};
    u32 height {};
    TextureFormat format {};
    std::vector<u8> data;
};

}
