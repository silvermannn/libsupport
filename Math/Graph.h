#pragma once

#include <vector>
#include <limits>
#include <cmath>

#include "Tensor.h"

template<typename W, typename V>
class Graph
{
private:
    static constexpr W noEdge = NAN;
    Tensor<W, V, 3> tensor;

public:
    struct Edge
    {
        V src = V(-1);
        V dest = V(-1);
        V label = V(-1);
        W weight = -std::numeric_limits<W>::infinity();
    };

    typedef std::vector<Edge> Edges;

    typedef V Vertex;

    typedef V Label;

    typedef W Weight;

    Graph(Vertex numVertices, Label numLabels)
        : tensor(noEdge, {numVertices, numVertices, numLabels})
    {
    }

    Graph(Vertex numVertices, Label numLabels, const Edges& edges)
        : tensor(noEdge, {numVertices, numVertices, numLabels})
    {
        std::for_each(edges.begin(), edges.end(), [&](const auto &e) { addEdge(e); });
    }

    Vertex numVertices() const
    {
        return tensor.sizeAt(0);
    }

    Label numLabels() const
    {
        return tensor.sizeAt(2);
    }

    static bool isEdge(W w)
    {
        return !std::isnan(w);
    }

    void addEdge(Vertex src, Vertex dest, Label label, W weight)
    {
        tensor.at(src, dest, label) = weight;
    }

    void addEdge(const Edge& e)
    {
        addEdge(e.src, e.dest, e.label, e.weight);
    }

    void removeEdge(Vertex src, Vertex dest, V label)
    {
        tensor.at(src, dest, label) = noEdge;
    }

    void removeEdge(Edge e)
    {
        removeEdge(e.src, e.dest, e.label);
    }

    W& weight(Vertex src, Vertex dest, Label label)
    {
        return tensor.at(src, dest, label);
    }

    const W& weight(Vertex src, Vertex dest, V label) const
    {
        return tensor.at(src, dest, label);
    }

    Edges edges() const
    {
        Edges res;
        res.reserve(numVertices());
        for (Vertex v1 = 0; v1 < numVertices(); ++v1)
        {
            for (Vertex v2 = 0; v2 < numVertices(); ++v2)
            {
                for (Label l = 0; l < numLabels(); ++l)
                {
                    const W& w = tensor.at(v1, v2, l);
                    if (isEdge(w))
                    {
                        res.push_back(Edge {v1, v2, l, w});
                    }
                }
            }
        }
        return res;
    }

    static void saveDot(std::ostream& stream, const Edges& edges)
    {
        stream << "digraph {" << std::endl;
        for (const auto& edge: edges)
        {
            if (edge.weight == 0)
            {
                stream << edge.src << " -> " << edge.dest <<  " [color=green4, label=\"" << edge.label << "\"]" << std::endl;
            }
            else
            {
                stream << edge.src << " -> " << edge.dest <<  " [color=lightblue, label=\""  << edge.label << " (" << edge.weight << ")\"]" << std::endl;
            }
        }
        stream << "}" << std::endl;
    }

    void saveDot(std::ostream& stream) const
    {
        saveDot(stream, edges());
    }

};
