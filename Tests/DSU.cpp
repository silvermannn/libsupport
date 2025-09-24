#include <gtest/gtest.h>

#include "../Math/DSU.h"

TEST(DSUTest, Create)
{
    constexpr size_t N = 31337;

    DSU<size_t> dsu(N);

    EXPECT_EQ(dsu.size(), N);

    for (size_t i = 0; i < N; ++i)
    {
        EXPECT_EQ(dsu.find(i), DSU<size_t>::notFound);
    }

    EXPECT_EQ(dsu.find(DSU<size_t>::notFound), DSU<size_t>::notFound);
}

TEST(DSUTest, Singletons)
{
    constexpr size_t N = 31337;

    DSU<size_t> dsu(N);

    EXPECT_EQ(dsu.size(), N);

    for (size_t i = 0; i < N; ++i)
    {
        dsu.singleton(i);
    }

    for (size_t i = 0; i < N; ++i)
    {
        EXPECT_EQ(dsu.find(i), i);
    }

    EXPECT_EQ(dsu.find(DSU<size_t>::notFound), DSU<size_t>::notFound);
}

TEST(DSUTest, UnionNotFounds)
{
    constexpr size_t N = 42;

    DSU<size_t> dsu(N);

    EXPECT_EQ(dsu.size(), N);

    for (size_t i = 0; i < N; i+=2)
    {
        dsu.makeUnion(i, i + 1);
    }

    for (size_t i = 0; i < N; i+=2)
    {
        EXPECT_EQ(dsu.find(i), dsu.find(i + 1));
    }

    EXPECT_EQ(dsu.find(DSU<size_t>::notFound), DSU<size_t>::notFound);
}

TEST(DSUTest, Union)
{
    constexpr size_t N = 1337;

    DSU<size_t> dsu(N);

    EXPECT_EQ(dsu.size(), N);

    for (size_t i = 0; i < N; ++i)
    {
        dsu.singleton(i);
    }

    for (size_t i = 1; i < N/2; ++i)
    {
        dsu.makeUnion(1, i);
    }

    for (size_t i = 1; i < N/2; ++i)
    {
        EXPECT_EQ(dsu.find(i), 1);
    }

    for (size_t i = N/2; i < N; ++i)
    {
        EXPECT_EQ(dsu.find(i), i);
    }

    EXPECT_EQ(dsu.find(0), 0);
    EXPECT_EQ(dsu.find(DSU<size_t>::notFound), DSU<size_t>::notFound);
}

TEST(DSUTest, UnionAll)
{
    constexpr size_t N = 31337;

    DSU<size_t> dsu(N);

    EXPECT_EQ(dsu.size(), N);

    for (size_t i = 0; i < N; ++i)
    {
        dsu.singleton(i);
    }

    for (size_t i = 0; i < N; ++i)
    {
        dsu.makeUnion(0, i);
    }

    for (size_t i = 0; i < N; ++i)
    {
        EXPECT_EQ(dsu.find(i), 0);
    }

    EXPECT_EQ(dsu.find(DSU<size_t>::notFound), DSU<size_t>::notFound);
}
