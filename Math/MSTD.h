#pragma once

#include <vector>
#include <unordered_set>
#include <optional>

#include "spdlog/spdlog.h"

#include "DSU.h"

template<typename G>
class ChuLiuEdmondsMST
{
    typedef G::Vertex Vertex;
    typedef G::Label Label;
    typedef G::Edge Edge;
    typedef G::Weight Weight;

    typedef G::Edges Edges;

    struct Path
    {
        std::vector<Edge> edges;
        std::unordered_set<Vertex> vertices;

        void print(const std::string& caption)
        {
            std::cout << caption << " [";
            std::for_each(edges.rbegin(), edges.rend(), [](const auto &e) {std::cout << e.src << "->" << e.dest << " ";});
            std::cout << "] (";
            std::for_each(vertices.begin(), vertices.end(), [](const Vertex &v) {std::cout << v << ", ";});
            std::cout << ")" << std::endl;
        }
    };

    G& graph;

    std::optional<Edge> maxIncomingWeight(Vertex v) const
    {
        Edge maxEdge;
        bool found = false;
        for (Vertex src = 0; src < graph.numVertices(); ++src)
        {
            for (Label l = 0; l < graph.numLabels(); ++l)
            {
                const Weight& w = graph.weight(src, v, l);
                if (w == 0)
                {
                    continue;
                }

                if (graph.isEdge(w) && w > maxEdge.weight)
                {
                    maxEdge.weight = w;
                    maxEdge.src = src;
                    maxEdge.dest = v;
                    maxEdge.label= l;

                    found = true;
                }
            }
        }
        return found ? std::make_optional(maxEdge) : std::optional<Edge>();
    }

    std::optional<Edge> maxIncomingWeightRestricted(Vertex v, Vertex alreadySrc) const
    {
        Edge maxEdge;
        bool found = false;
        for (Vertex src = 0; src < graph.numVertices(); ++src)
        {
            if (src == alreadySrc)
            {
                continue;
            }

            for (Label l = 0; l < graph.numLabels(); ++l)
            {
                const Weight& w = graph.weight(src, v, l);
                if (w == 0)
                {
                    continue;
                }

                if (graph.isEdge(w) && w > maxEdge.weight)
                {
                    maxEdge.weight = w;
                    maxEdge.src = src;
                    maxEdge.dest = v;
                    maxEdge.label= l;

                    found = true;
                }
            }
        }
        return found ? std::make_optional(maxEdge) : std::optional<Edge>();
    }

    void subtractWeightFromIncomingNodes(Vertex v, Weight w)
    {
        for (Vertex src = 0; src < graph.numVertices(); ++src)
        {
            for (Label l = 0; l < graph.numLabels(); ++l)
            {
                graph.weight(src, v, l) -= w;
            }
        }
    }

    void removeEdges()
    {
        for (Vertex dest = 0; dest < graph.numVertices(); ++dest)
        {
            for (Vertex src = 0; src < graph.numVertices(); ++src)
            {
                for (Label l = 0; l < graph.numLabels(); ++l)
                {
                    graph.removeEdge(src, dest, l);
                }
            }
        }
    }

public:
    ChuLiuEdmondsMST(G& _graph)
        : graph(_graph)
    {
    }

    // Chu-Liu Edmonds Maximum Spanning Tree algorithm
    std::optional<Edges> getSpanningTree(Vertex root)
    {
        spdlog::debug("Get maximual spanning tree, {} vertices, {} labels", graph.numVertices(), graph.numLabels());
        Edges found;

        for (Vertex src = 0; src < graph.numVertices(); ++src)
        {
            for (Label l = 0; l < graph.numLabels(); ++l)
            {
                graph.removeEdge(src, root, l);
                graph.removeEdge(src, src, l);
            }
        }

        DSU<Vertex> dsu(graph.numVertices());

        dsu.singleton(root);

        std::vector<Path> cycles;

        for (Vertex dest = 0; dest < graph.numVertices(); ++dest)
        {
            spdlog::debug("Processing vertex {}", dest);
            if (dsu.find(dest) != dsu.notFound)
            {
                spdlog::debug("{} found at DSU {}, continue", dest, dsu.find(dest));
                continue;
            }

            const auto& res = maxIncomingWeight(dest);
            if (!res)
            {
                spdlog::debug("No maximum incoming weight for destinatoin vertex {}", dest);
                return {};
            }

            const Edge e = *res;
            found.push_back(e);
            subtractWeightFromIncomingNodes(dest, e.weight);

            Path path({{1, e}, {dest}});
            Vertex src = e.src;
            while(dsu.find(src) == dsu.notFound && !path.vertices.contains(src))
            {
                path.vertices.insert(src);
                const auto& res = maxIncomingWeight(src);
                if (!res)
                {
                    spdlog::debug("No maximum incoming weight for destinatoin vertex {}", src);
                    return {};
                }
                spdlog::debug("{}, maximum weight {}, continue up", src, e.weight);
                const Edge e = *res;
                found.push_back(e);
                subtractWeightFromIncomingNodes(src, e.weight);
                src = e.src;
                path.edges.push_back(e);
            }

            if (dsu.find(src) == dsu.find(root))
            {
                spdlog::debug("{}, went to root {}", src, root);
                std::for_each(path.edges.begin(), path.edges.end(), [&](const auto &e) {dsu.makeUnion(e.dest, root);});
            }
            else if (path.vertices.contains(src))
            {
                spdlog::debug("{}, found cycle {}", src, path.edges.size());
                std::for_each(path.edges.begin(), path.edges.end(), [&](const auto &e) {dsu.makeUnion(e.dest, dest);});
                cycles.push_back(path);
            }
            else
            {
                if (dsu.find(src) == dsu.notFound)
                {
                    dsu.singleton(src);
                }
                spdlog::debug("{}, found group {} for {}", src, path.edges.size(), dsu.find(src));
                std::for_each(path.edges.begin(), path.edges.end(), [&](const auto &e) {dsu.makeUnion(e.dest, src);});
            }

        }

        spdlog::debug("Found {} cycles", cycles.size());
        for (auto& cycle: cycles)
        {
            Vertex src = cycle.edges[cycle.edges.size() - 1].src;
            bool erasing = false;
            for(size_t i = cycle.edges.size() - 2; i != 0; --i)
            {
                if (!erasing && cycle.edges[i].dest == src)
                {
                    erasing = true;
                }
                if (erasing)
                {
                    cycle.edges.erase(cycle.edges.begin()); ///rewrite it
                }
            }

            Weight maxWeight = -std::numeric_limits<Weight>::infinity();
            Edge maxEdge;
            for (auto& edge: cycle.edges)
            {
                const auto& res = maxIncomingWeightRestricted(edge.dest, edge.src);
                if (!res)
                {
                    spdlog::debug("No maximum incoming weight for destinatoin vertex {} <- {}", edge.dest, edge.src);
                    return {};
                }

                const Edge e = *res;
                if (e.weight > maxWeight)
                {
                    maxWeight = e.weight;
                    maxEdge = e;
                }

            }
            subtractWeightFromIncomingNodes(maxEdge.dest, maxEdge.weight);
            for(size_t i = 0; i < found.size(); ++i)
            {
                if (found[i].dest == maxEdge.dest)
                {
                    maxEdge.weight += found[i].weight;
                    spdlog::debug("Erasing {} {} {} = {}", found[i].src, found[i].dest, found[i].label, found[i].weight);
                    found.erase(found.begin() + i);
                    break;
                }
            }
            spdlog::debug("Adding {} {} {} = {}", maxEdge.src, maxEdge.dest, maxEdge.label, maxEdge.weight);
            found.push_back(maxEdge);
        }

        spdlog::debug("Found {} edges:", found.size());
        removeEdges();
        for (auto& edge: found)
        {
            spdlog::debug("{} {} {} {}", edge.src, edge.dest, edge.label, edge.weight);
            graph.addEdge(edge);
        }

        return std::make_optional(found);
    }
};
