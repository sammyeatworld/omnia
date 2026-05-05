/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <gtest/gtest.h>

#include <LibMath/Vec2.h>

TEST(Vec2, Construction)
{
    Math::Vec2f vec(1.0F, 2.0F);
    EXPECT_EQ(vec.x, 1.0F);
    EXPECT_EQ(vec.y, 2.0F);

    Math::Vec2f vec2(vec);
    EXPECT_EQ(vec2.x, 1.0F);
    EXPECT_EQ(vec2.y, 2.0F);
}

TEST(Vec2, BasicOperations)
{
    Math::Vec2f vec1(1.0F, 2.0F);
    Math::Vec2f vec2(3.0F, 4.0F);

    auto sum = vec1 + vec2;
    EXPECT_EQ(sum.x, 4.0F);
    EXPECT_EQ(sum.y, 6.0F);

    auto difference = vec1 - vec2;
    EXPECT_EQ(difference.x, -2.0F);
    EXPECT_EQ(difference.y, -2.0F);

    auto multiplied = vec1 * 2.0F;
    EXPECT_EQ(multiplied.x, 2.0F);
    EXPECT_EQ(multiplied.y, 4.0F);

    auto divided = vec1 / 2.0F;
    EXPECT_EQ(divided.x, 0.5F);
    EXPECT_EQ(divided.y, 1.0F);
}

TEST(Vec2, CompoundOperations)
{
    Math::Vec2f vec(1.0F, 2.0F);

    vec += Math::Vec2f(3.0F, 4.0F);
    EXPECT_EQ(vec.x, 4.0F);
    EXPECT_EQ(vec.y, 6.0F);

    vec -= Math::Vec2f(1.0F, 1.0F);
    EXPECT_EQ(vec.x, 3.0F);
    EXPECT_EQ(vec.y, 5.0F);

    vec *= 2.0F;
    EXPECT_EQ(vec.x, 6.0F);
    EXPECT_EQ(vec.y, 10.0F);

    vec /= 2.0F;
    EXPECT_EQ(vec.x, 3.0F);
    EXPECT_EQ(vec.y, 5.0F);
}

TEST(Vec2, Length)
{
    Math::Vec2f vec(3.0F, 4.0F);
    EXPECT_EQ(vec.length(), 5.0F);
}

TEST(Vec2, Normalize)
{
    Math::Vec2f vec(3.0F, 4.0F);
    vec.normalize();
    EXPECT_EQ(vec.length(), 1.0F);
}

TEST(Vec2, DotProduct)
{
    Math::Vec2f vec1(1.0F, 2.0F);
    Math::Vec2f vec2(3.0F, 4.0F);
    EXPECT_EQ(dot(vec1, vec2), 11.0F);
}

TEST(Vec2, CrossProduct)
{
    Math::Vec2f vec1(1.0F, 2.0F);
    Math::Vec2f vec2(3.0F, 4.0F);
    EXPECT_EQ(cross(vec1, vec2), -2.0F);
}