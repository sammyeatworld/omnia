/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <vector>

#include <Common/Noncopyable.h>
#include <LibMath/Vec3.h>

namespace RHI {

enum class TextureUsage : u8 {
    Sampled         = 1 << 0,
    ColorAttachment = 1 << 1,
    DepthStencil    = 1 << 2
};

enum class TextureFormat : u8 {
    Unknown = 0,
    R16G16B16A16_SFLOAT,
    R8G8B8A8_SRGB,
    R8G8B8A8_UNORM,
    D32_SFLOAT,
};

class Texture {
    OA_MAKE_NONCOPYABLE(Texture);
    OA_MAKE_DEFAULT_MOVABLE(Texture);

public:
    struct Configuration {
        u32 width {};
        u32 height {};
        TextureFormat format {};
        TextureUsage usage = TextureUsage::Sampled;
        std::vector<u8> data;
    };

    virtual ~Texture() = default;

    virtual auto width() const -> u32 = 0;
    virtual auto height() const -> u32 = 0;
    virtual auto format() const -> TextureFormat = 0;
protected:
    Texture() = default;
};

static constexpr auto operator|(TextureUsage lhs, TextureUsage rhs) -> TextureUsage
{
    return static_cast<TextureUsage>(static_cast<u8>(lhs) | static_cast<u8>(rhs));
}

static constexpr auto operator&(TextureUsage lhs, TextureUsage rhs) -> TextureUsage
{
    return static_cast<TextureUsage>(static_cast<u8>(lhs) & static_cast<u8>(rhs));
}

static constexpr auto any(TextureUsage usage) -> bool
{
    return static_cast<u8>(usage) != 0;
}

}
