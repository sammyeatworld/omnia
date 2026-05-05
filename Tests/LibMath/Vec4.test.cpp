/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <gtest/gtest.h>

#include <LibMath/Vec4.h>

TEST(Vec4, Construction)
{
    Math::Vec4f vec(1.0F, 2.0F, 3.0F, 4.0F);
    EXPECT_EQ(vec.x, 1.0F);
    EXPECT_EQ(vec.y, 2.0F);
    EXPECT_EQ(vec.z, 3.0F);
    EXPECT_EQ(vec.w, 4.0F);

    Math::Vec4f vec2(vec);
    EXPECT_EQ(vec2.x, 1.0F);
    EXPECT_EQ(vec2.y, 2.0F);
    EXPECT_EQ(vec2.z, 3.0F);
    EXPECT_EQ(vec2.w, 4.0F);
}

TEST(Vec4, BasicOperations)
{
    Math::Vec4f vec1(1.0F, 2.0F, 3.0F, 4.0F);
    Math::Vec4f vec2(5.0F, 6.0F, 7.0F, 8.0F);

    auto sum = vec1 + vec2;
    EXPECT_EQ(sum.x, 6.0F);
    EXPECT_EQ(sum.y, 8.0F);
    EXPECT_EQ(sum.z, 10.0F);
    EXPECT_EQ(sum.w, 12.0F);

    auto difference = vec1 - vec2;
    EXPECT_EQ(difference.x, -4.0F);
    EXPECT_EQ(difference.y, -4.0F);
    EXPECT_EQ(difference.z, -4.0F);
    EXPECT_EQ(difference.w, -4.0F);

    auto multiplied = vec1 * 2.0F;
    EXPECT_EQ(multiplied.x, 2.0F);
    EXPECT_EQ(multiplied.y, 4.0F);
    EXPECT_EQ(multiplied.z, 6.0F);
    EXPECT_EQ(multiplied.w, 8.0F);

    auto divided = vec1 / 2.0F;
    EXPECT_EQ(divided.x, 0.5F);
    EXPECT_EQ(divided.y, 1.0F);
    EXPECT_EQ(divided.z, 1.5F);
    EXPECT_EQ(divided.w, 2.0F);
}

TEST(Vec4, CompoundOperations)
{
    Math::Vec4f vec(1.0F, 2.0F, 3.0F, 4.0F);

    vec += Math::Vec4f(5.0F, 6.0F, 7.0F, 8.0F);
    EXPECT_EQ(vec.x, 6.0F);
    EXPECT_EQ(vec.y, 8.0F);
    EXPECT_EQ(vec.z, 10.0F);
    EXPECT_EQ(vec.w, 12.0F);

    vec -= Math::Vec4f(1.0F, 1.0F, 1.0F, 1.0F);
    EXPECT_EQ(vec.x, 5.0F);
    EXPECT_EQ(vec.y, 7.0F);
    EXPECT_EQ(vec.z, 9.0F);
    EXPECT_EQ(vec.w, 11.0F);

    vec *= 2.0F;
    EXPECT_EQ(vec.x, 10.0F);
    EXPECT_EQ(vec.y, 14.0F);
    EXPECT_EQ(vec.z, 18.0F);
    EXPECT_EQ(vec.w, 22.0F);

    vec /= 2.0F;
    EXPECT_EQ(vec.x, 5.0F);
    EXPECT_EQ(vec.y, 7.0F);
    EXPECT_EQ(vec.z, 9.0F);
    EXPECT_EQ(vec.w, 11.0F);
}

TEST(Vec4, length)
{
    Math::Vec4f vec(1.0F, 2.0F, 3.0F, 4.0F);
    EXPECT_NEAR(vec.length(), 5.4F, 0.1F);
}

TEST(Vec4, normalize)
{
    Math::Vec4f vec(1.0F, 2.0F, 3.0F, 4.0F);
    vec.normalize();
    EXPECT_NEAR(vec.length(), 1.0F, 0.0001F);
}