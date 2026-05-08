/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <Common/Noncopyable.h>
#include <Common/Types.h>
#include <LibRHI/Forward.h>

namespace RHI {

class RenderTarget {
    OA_MAKE_NONCOPYABLE(RenderTarget);
    OA_MAKE_DEFAULT_MOVABLE(RenderTarget);

public:
    struct Configuration {
        RenderPass const* render_pass;
        std::vector<Texture const*> textures;
        Texture const* depth_texture {};
    };

    virtual ~RenderTarget() = default;

    virtual auto width() const -> u32 = 0;
    virtual auto height() const -> u32 = 0;
protected:
    RenderTarget() = default;
};

}
