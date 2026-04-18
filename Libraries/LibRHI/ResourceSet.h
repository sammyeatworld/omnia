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

class ResourceSet {
    OA_MAKE_NONCOPYABLE(ResourceSet);
    OA_MAKE_DEFAULT_MOVABLE(ResourceSet);

public:
    struct Configuration {
        ResourceLayout const* layout;
    };

    virtual ~ResourceSet() = default;

    virtual void set_uniform_buffer(u32 binding, Buffer const* buffer) = 0;
    virtual void set_texture(u32 binding, Texture const* texture) = 0;
protected:
    ResourceSet() = default;
};

}
