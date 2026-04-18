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

class CommandBuffer {
    OA_MAKE_NONCOPYABLE(CommandBuffer);
    OA_MAKE_DEFAULT_MOVABLE(CommandBuffer);

public:
    virtual ~CommandBuffer() = default;

    virtual void reset() const = 0;

    virtual void begin() const = 0;
    virtual void end() const = 0;

    virtual void begin_render_pass(RenderPass const* render_pass, RenderTarget const* render_target) const = 0;
    virtual void end_render_pass() const = 0;

    virtual void bind_vertex_buffer(Buffer const* vertex_buffer) const = 0;
    virtual void bind_pipeline(Pipeline const* pipeline) const = 0;

    virtual void draw(u32 vertex_count, u32 instance_count, u32 first_vertex, u32 first_instance) const = 0;

    virtual void set_viewport(u32 x, u32 y, u32 width, u32 height) const = 0;
    virtual void set_scissor(u32 x, u32 y, u32 width, u32 height) const = 0;
protected:
    CommandBuffer() = default;
};

}
