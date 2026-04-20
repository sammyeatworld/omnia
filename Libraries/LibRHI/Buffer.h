/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <expected>
#include <string>

#include <Common/Noncopyable.h>
#include <Common/Types.h>

namespace RHI {

enum class BufferUsage : u8 {
    Vertex = 0,
    Index,
    Uniform,
    Storage,
};

class Buffer {
    OA_MAKE_NONCOPYABLE(Buffer);
    OA_MAKE_DEFAULT_MOVABLE(Buffer);

public:
    struct Configuration {
        u64 size {};
        BufferUsage usage {};
        void const* data {};
    };

    virtual ~Buffer() = default;

    virtual void set_data(void const* data, u64 size) = 0;
protected:
    Buffer() = default;
};

}
