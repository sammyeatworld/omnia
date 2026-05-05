/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <gtest/gtest.h>

#include <LibMath/Math.h>

TEST(Mat4, Construction)
{
    auto mat = Math::Mat4f::identity();
    EXPECT_EQ(mat[0], 1);
    EXPECT_EQ(mat[5], 1);
    EXPECT_EQ(mat[10], 1);
    EXPECT_EQ(mat[15], 1);

    Math::Mat4f mat2({
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16
    });
    for (size_t i = 0; i < 16; i++) {
        EXPECT_EQ(mat2[i], i + 1);
    }
}

TEST(Mat4, Translation)
{
    auto mat = Math::Mat4f::identity();
    auto translated = mat.translate({ 1.0F, 2.0F, 3.0F });
    EXPECT_EQ(translated[12], 1.0F);
    EXPECT_EQ(translated[13], 2.0F);
    EXPECT_EQ(translated[14], 3.0F);
}

TEST(Mat4, Scaling)
{
    auto mat = Math::Mat4f::scale({ 2.0F, 3.0F, 4.0F });
    EXPECT_EQ(mat[0], 2.0F);
    EXPECT_EQ(mat[5], 3.0F);
    EXPECT_EQ(mat[10], 4.0F);
}

TEST(Mat4, PerspectiveSymmetry)
{
    auto aspect = 1.0F;
    auto near = 0.1F;
    auto far = 10.0F;

    auto projection = Math::Mat4f::perspective(DEG_TO_RAD(90.0F), aspect, near, far);

    EXPECT_EQ(projection[1], 0.0F);
    EXPECT_EQ(projection[2], 0.0F);
    EXPECT_EQ(projection[4], 0.0F);
    EXPECT_EQ(projection[6], 0.0F);
}

TEST(Mat4, PerspectiveFovScaling)
{
    float aspect = 1.0F;
    float near = 0.1F;
    float far = 100.0F;

    auto m1 = Math::Mat4f::perspective(DEG_TO_RAD(45.0F), aspect, near, far);
    auto m2 = Math::Mat4f::perspective(DEG_TO_RAD(90.0F), aspect, near, far);

    EXPECT_GT(m1[0], m2[0]);
}

TEST(Mat4, PerspectiveDepthMapping)
{
    float aspect = 1.0F;
    float near = 0.1F;
    float far = 100.0F;

    auto projection = Math::Mat4f::perspective(DEG_TO_RAD(60.0F), aspect, near, far);

    EXPECT_LT(projection[10], 0.0F);
    EXPECT_LT(projection[10], 0.0F);
}

TEST(Mat4, FromQuaternionIdentity)
{
    auto quat = Math::Quatf::identity();
    auto mat = Math::Mat4f::from_quaternion(quat);

    EXPECT_EQ(mat[0], 1.0F);
    EXPECT_EQ(mat[5], 1.0F);
    EXPECT_EQ(mat[10], 1.0F);
    EXPECT_EQ(mat[15], 1.0F);
}
