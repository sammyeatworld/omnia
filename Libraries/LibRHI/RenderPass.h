/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <variant>
#include <vector>

#include <Common/Noncopyable.h>
#include <Common/Types.h>
#include <LibMath/Vec4.h>
#include <LibRHI/Forward.h>
#include <LibRHI/Texture.h>

namespace RHI {

class RenderPass {
    OA_MAKE_NONCOPYABLE(RenderPass);
    OA_MAKE_DEFAULT_MOVABLE(RenderPass);

public:
    enum class LoadOp : u8 {
        Load = 0,
        Clear,
        DontCare
    };

    enum class StoreOp : u8 {
        Store = 0,
        DontCare
    };

    struct Configuration {
        struct Attachment {
            Texture::Format format {};
            LoadOp load_op {};
            StoreOp store_op {};
            Math::Vec4f clear_color;
        };

        std::vector<Attachment> color_attachments;
    };

    virtual ~RenderPass() = default;

    virtual void begin(CommandBuffer const* command_buffer, RenderTarget const* render_target) const = 0;
    virtual void end(CommandBuffer const* command_buffer) const = 0;
protected:
    RenderPass() = default;
};

}
