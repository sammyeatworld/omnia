/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#define TRY_ASSIGN(lhs, expected)                              \
    {                                                          \
        auto result = (expected);                              \
        if (!result.has_value()) {                             \
            return std::unexpected(std::move(result).error()); \
        }                                                      \
        lhs = std::move(result).value();                       \
    }                                                          \
    (void)0
