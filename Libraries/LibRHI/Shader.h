/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <vector>

#include <Common/Noncopyable.h>
#include <Common/Types.h>

namespace RHI {

class Shader {
    OA_MAKE_NONCOPYABLE(Shader);
    OA_MAKE_DEFAULT_MOVABLE(Shader);

public:
    enum class Format : u8 {
        SPIRV = 0,
        DXIL,
        MetalIR
    };

    enum class Stage : u8 {
        Vertex = 0,
        Fragment
    };

    struct Variant {
        Format format;
        std::vector<u8> bytecode;
    };

    struct Configuration {
        Stage stage;
        std::vector<Variant> variants;
    };

    virtual ~Shader() = default;

    virtual auto config() const -> Configuration const& = 0;
protected:
    Shader() = default;
};

}
