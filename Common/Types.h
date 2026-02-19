/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <cstddef>
#include <Common/Platform.h>

using u64 = __UINT64_TYPE__;
using u32 = __UINT32_TYPE__;
using u16 = __UINT16_TYPE__;
using u8 = __UINT8_TYPE__;
using i64 = __INT64_TYPE__;
using i32 = __INT32_TYPE__;
using i16 = __INT16_TYPE__;
using i8 = __INT8_TYPE__;

using f16 = _Float16;
static_assert(__FLT16_MANT_DIG__ == 11 && __FLT16_MAX_EXP__ == 16);

using f32 = float;
static_assert(__FLT_MANT_DIG__ == 24 && __FLT_MAX_EXP__ == 128);

using f64 = double;
static_assert(__DBL_MANT_DIG__ == 53 && __DBL_MAX_EXP__ == 1024);

constexpr u64 KiB = 1024;
constexpr u64 MiB = KiB * KiB;
constexpr u64 GiB = KiB * KiB * KiB;
constexpr u64 TiB = KiB * KiB * KiB * KiB;
constexpr u64 PiB = KiB * KiB * KiB * KiB * KiB;
constexpr u64 EiB = KiB * KiB * KiB * KiB * KiB * KiB;

namespace Common
{
    constexpr size_t align_down_to(size_t const value, size_t const alignment)
    {
        return value & ~(alignment - 1);
    }

    constexpr size_t align_up_to(size_t const value, size_t const alignment)
    {
        return (value + (alignment - 1)) & ~(alignment - 1);
    }

    enum class [[nodiscard]] TriState : u8
    {
        False,
        True,
        Unknown
    };

    enum class MemoryOrder: u8
    {
        Relaxed = __ATOMIC_RELAXED,
        Consume = __ATOMIC_CONSUME,
        Acquire = __ATOMIC_ACQUIRE,
        Release = __ATOMIC_RELEASE,
        AcqRel = __ATOMIC_ACQ_REL,
        SeqCst = __ATOMIC_SEQ_CST
    };
}

#if USING_OA_GLOBALLY
using Common::align_down_to;
using Common::align_up_to;
using Common::MemoryOrder;
using Common::TriState;
#endif
