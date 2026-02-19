/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#define OA_MAKE_NONCOPYABLE(c) \
private:                       \
c(c const&) = delete;          \
c& operator=(c const&) = delete

#define OA_MAKE_NONMOVABLE(c) \
private:                      \
c(c&&) = delete;              \
c& operator=(c&&) = delete

#define OA_MAKE_DEFAULT_MOVABLE(c) \
public:                            \
c(c&&) = default;                  \
c& operator=(c&&) = default

#define OA_MAKE_DEFAULT_COPYABLE(c) \
public:                             \
c(c const&) = default;              \
c& operator=(c const&) = default
