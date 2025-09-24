#include <gtest/gtest.h>

#include <fstream>

#include "../Math/Graph.h"

TEST(GraphTest, CreateEmpty)
{
    constexpr size_t vertices = 7;
    constexpr size_t labels = 9;

    Graph<float, size_t> g(vertices, labels);

    EXPECT_EQ(g.numVertices(), vertices);
    EXPECT_EQ(g.numLabels(), labels);
    EXPECT_EQ(g.edges().size(), 0);
}

TEST(GraphTest, CreateFromEdges)
{
    constexpr size_t vertices = 7;
    constexpr size_t labels = 2;

    Graph<float, size_t>::Edges edges;
    edges.push_back({0, 1, 0, 0.2});
    edges.push_back({2, 1, 1, 0.5});

    Graph<float, size_t> g(vertices, labels, edges);

    EXPECT_EQ(g.numVertices(), vertices);
    EXPECT_EQ(g.numLabels(), labels);

    EXPECT_EQ(g.edges().size(), edges.size());
}

TEST(GraphTest, Edges)
{
    constexpr size_t vertices = 7;
    constexpr size_t labels = 2;

    Graph<float, size_t> g(vertices, labels);

    EXPECT_EQ(g.numVertices(), vertices);
    EXPECT_EQ(g.numLabels(), labels);
    EXPECT_EQ(g.edges().size(), 0);

    g.addEdge(0, 1, 0, 0.2);
    g.addEdge(2, 1, 1, 0.5);

    EXPECT_EQ(g.edges().size(), 2);
}
