/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <numbers>

#include <LibMath/Mat4.h>
#include <LibMath/Quat.h>
#include <LibMath/Vec2.h>
#include <LibMath/Vec3.h>
#include <LibMath/Vec4.h>

#define DEG_TO_RAD(angle_in_degrees) ((angle_in_degrees) * std::numbers::pi_v<f32> / 180.0F)
#define RAD_TO_DEG(angle_in_radians) ((angle_in_radians) * 180.0F / std::numbers::pi_v<f32>)
