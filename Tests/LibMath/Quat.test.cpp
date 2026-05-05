/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <gtest/gtest.h>

#include <LibMath/Math.h>

TEST(Quat, Construction)
{
    auto quat = Math::Quatf::identity();
    EXPECT_EQ(quat.x, 0);
    EXPECT_EQ(quat.y, 0);
    EXPECT_EQ(quat.z, 0);
    EXPECT_EQ(quat.w, 1);

    Math::Quatf quat2(1.0F, 2.0F, 3.0F, 4.0F);
    EXPECT_EQ(quat2.x, 1.0F);
    EXPECT_EQ(quat2.y, 2.0F);
    EXPECT_EQ(quat2.z, 3.0F);
    EXPECT_EQ(quat2.w, 4.0F);
}

TEST(Quat, Length)
{
    Math::Quatf quat(1.0F, 2.0F, 3.0F, 4.0F);
    auto expected = std::sqrt(1.0F + 4.0F + 9.0F + 16.0F);
    EXPECT_EQ(quat.length(), expected);
}

TEST(Quat, Normalize)
{
    Math::Quatf quat(1.0F, 2.0F, 3.0F, 4.0F);
    quat.normalize();
    auto length = quat.length();

    EXPECT_EQ(length, 1.0F);
}

TEST(Quat, MultiplicationAssociativity)
{
    Math::Quatf a(1.0F, 2.0F, 3.0F, 4.0F);
    Math::Quatf b(5.0F, 6.0F, 7.0F, 8.0F);
    Math::Quatf c(9.0F, 10.0F, 11.0F, 12.0F);

    auto ab_c = (a * b) * c;
    auto a_bc = a * (b * c);

    EXPECT_EQ(ab_c.x, a_bc.x);
    EXPECT_EQ(ab_c.y, a_bc.y);
    EXPECT_EQ(ab_c.z, a_bc.z);
    EXPECT_EQ(ab_c.w, a_bc.w);
}

TEST(Quat, AxisAngleZeroAxis)
{
    Math::Vec3f axis(0.0F, 0.0F, 0.0F);
    auto quat = Math::Quatf::from_axis_angle(axis, 1.0F);
    EXPECT_EQ(quat, Math::Quatf::identity());
}

TEST(Quat, ZRotation)
{
    Math::Vec3f axis(0.0F, 0.0F, 1.0F);
    auto quat = Math::Quatf::from_axis_angle(axis, DEG_TO_RAD(90.0F));

    EXPECT_NEAR(quat.z, 0.7071F, 1e-4F);
    EXPECT_NEAR(quat.w, 0.7071F, 1e-4F);
}