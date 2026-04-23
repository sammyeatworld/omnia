/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <Common/Noncopyable.h>
#include <Common/Types.h>
#include <LibGraphics/ShaderTypes.h>

namespace RHI {

// TODO: Split texture and sampler
enum class ResourceType : u8 {
    UniformBuffer = 0,
    Texture
};

class ResourceLayout {
    OA_MAKE_NONCOPYABLE(ResourceLayout);
    OA_MAKE_DEFAULT_MOVABLE(ResourceLayout);

public:
    struct Binding {
        u32 binding;
        ResourceType type;
        ShaderStage stage;
    };

    struct Configuration {
        std::vector<Binding> bindings;
    };

    virtual ~ResourceLayout() = default;
protected:
    ResourceLayout() = default;
};

}
