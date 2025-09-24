#include <gtest/gtest.h>

#include <fstream>

#include "../Math/MSTD.h"
#include "../Math/Graph.h"

TEST(MSTDTest, SpanningTree1Cycle)
{
    constexpr size_t vertices = 6;
    constexpr size_t labels = 1;

    Graph<float, size_t> g(vertices, labels);

    EXPECT_EQ(g.numVertices(), vertices);
    EXPECT_EQ(g.numLabels(), labels);
    EXPECT_EQ(g.edges().size(), 0);

    constexpr size_t root = 0;

    for (size_t l = 0; l < labels; ++l)
    {
        g.addEdge(root, 1, l, 1.0f);
        g.addEdge(2, 1, l, 3.0f);
        g.addEdge(3, 1, l, 2.0f);

        g.addEdge(root, 2, l, 1.0f);
        g.addEdge(1, 2, l, 3.0f);
        g.addEdge(3, 2, l, 2.0f);

        g.addEdge(root, 3, l, 3.0f);
        g.addEdge(1, 3, l, 2.0f);
        g.addEdge(2, 3, l, 2.0f);

        g.addEdge(1, 5, l, 2.0f);

        g.addEdge(root, 4, l, 12.0f);
    }

    EXPECT_EQ(g.edges().size(), 11);

    std::ofstream f("./1cycle.dot");
    g.saveDot(f);

    ChuLiuEdmondsMST<Graph<float, size_t>> mst(g);

    auto res = mst.getSpanningTree(root);

    EXPECT_TRUE(res);
    EXPECT_EQ(res->size(), vertices - 1);
}

TEST(MSTDTest, SpanningTreeRandom)
{
    constexpr size_t vertices = 313;
    constexpr size_t labels = 37;

    Graph<float, size_t> g(vertices, labels);

    EXPECT_EQ(g.numVertices(), vertices);
    EXPECT_EQ(g.numLabels(), labels);
    EXPECT_EQ(g.edges().size(), 0);

    for (size_t t = 0; t < 10; ++t)
    {
        constexpr size_t root = 0;

        for (size_t src = 0; src < vertices; ++src)
        {
            for (size_t dest = 0; dest < vertices; ++dest)
            {
                for (size_t l = 0; l < labels; ++l)
                {
                    g.addEdge(src, dest, l, 1 + rand() % (labels * vertices));
                }
            }
        }

        EXPECT_EQ(g.edges().size(), vertices * vertices * labels);

        ChuLiuEdmondsMST<Graph<float, size_t>> mst(g);

        auto res = mst.getSpanningTree(root);
        EXPECT_TRUE(res);
        EXPECT_EQ(res->size(), vertices - 1);

        if (!res || res->size() != vertices - 1)
        {
            std::ofstream f("./random.dot");
            g.saveDot(f);
            break;
        }
    }
}
