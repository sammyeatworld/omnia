/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <Common/Noncopyable.h>

namespace RHI {

class CommandBuffer {
public:
    virtual ~CommandBuffer() = default;

    virtual void reset() const = 0;
    virtual void begin() const = 0;
    virtual void end() const = 0;
protected:
    CommandBuffer() = default;
};

}
