/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <gtest/gtest.h>

#include <LibMath/Vec3.h>

TEST(Vec3, Construction)
{
    Math::Vec3f vec(1.0F, 2.0F, 3.0F);
    EXPECT_EQ(vec.x, 1.0F);
    EXPECT_EQ(vec.y, 2.0F);
    EXPECT_EQ(vec.z, 3.0F);

    Math::Vec3f vec2(vec);
    EXPECT_EQ(vec2.x, 1.0F);
    EXPECT_EQ(vec2.y, 2.0F);
    EXPECT_EQ(vec2.z, 3.0F);
}

TEST(Vec3, BasicOperations)
{
    Math::Vec3f vec1(1.0F, 2.0F, 3.0F);
    Math::Vec3f vec2(4.0F, 5.0F, 6.0F);

    auto sum = vec1 + vec2;
    EXPECT_EQ(sum.x, 5.0F);
    EXPECT_EQ(sum.y, 7.0F);
    EXPECT_EQ(sum.z, 9.0F);

    auto difference = vec1 - vec2;
    EXPECT_EQ(difference.x, -3.0F);
    EXPECT_EQ(difference.y, -3.0F);
    EXPECT_EQ(difference.z, -3.0F);

    auto multiplied = vec1 * 2.0F;
    EXPECT_EQ(multiplied.x, 2.0F);
    EXPECT_EQ(multiplied.y, 4.0F);
    EXPECT_EQ(multiplied.z, 6.0F);

    auto divided = vec1 / 2.0F;
    EXPECT_EQ(divided.x, 0.5F);
    EXPECT_EQ(divided.y, 1.0F);
    EXPECT_EQ(divided.z, 1.5F);
}

TEST(Vec3, CompoundOperations)
{
    Math::Vec3f vec(1.0F, 2.0F, 3.0F);

    vec += Math::Vec3f(4.0F, 5.0F, 6.0F);
    EXPECT_EQ(vec.x, 5.0F);
    EXPECT_EQ(vec.y, 7.0F);
    EXPECT_EQ(vec.z, 9.0F);

    vec -= Math::Vec3f(1.0F, 1.0F, 1.0F);
    EXPECT_EQ(vec.x, 4.0F);
    EXPECT_EQ(vec.y, 6.0F);
    EXPECT_EQ(vec.z, 8.0F);

    vec *= 2.0F;
    EXPECT_EQ(vec.x, 8.0F);
    EXPECT_EQ(vec.y, 12.0F);
    EXPECT_EQ(vec.z, 16.0F);

    vec /= 2.0F;
    EXPECT_EQ(vec.x, 4.0F);
    EXPECT_EQ(vec.y, 6.0F);
    EXPECT_EQ(vec.z, 8.0F);
}

TEST(Vec3, Length)
{
    Math::Vec3f vec(2.0F, 3.0F, 6.0F);
    EXPECT_EQ(vec.length(), 7.0F);
}

TEST(Vec3, Normalize)
{
    Math::Vec3f vec(2.0F, 3.0F, 6.0F);
    vec.normalize();
    EXPECT_NEAR(vec.length(), 1.0F, 0.0001F);
}

TEST(Vec3, DotProduct)
{
    Math::Vec3f vec1(1.0F, 2.0F, 3.0F);
    Math::Vec3f vec2(4.0F, 5.0F, 6.0F);
    EXPECT_EQ(dot(vec1, vec2), 32.0F);
}

TEST(Vec3, CrossProduct)
{
    Math::Vec3f vec1(1.0F, 2.0F, 3.0F);
    Math::Vec3f vec2(4.0F, 5.0F, 6.0F);
    auto crossProduct = cross(vec1, vec2);
    EXPECT_EQ(crossProduct.x, -3.0F);
    EXPECT_EQ(crossProduct.y, 6.0F);
    EXPECT_EQ(crossProduct.z, -3.0F);
}