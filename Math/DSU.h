#pragma once

#include <vector>
#include <iostream>

// http://e-maxx.ru/algo/dsu
template<typename T>
class DSU
{
    std::vector<T> parent;
    std::vector<T> rank;

public:
    static constexpr T notFound = T(-1);

    DSU(T numElems)
        : parent(numElems, notFound)
        , rank(numElems, 0)
    {
    }

    T size() const
    {
        return parent.size();
    }

    void singleton(T e)
    {
        parent[e] = e;
        rank[e] = 0;
    }

    void makeUnion(T e1, T e2)
    {
        T f1 = find(e1);
        if (f1 == notFound)
        {
            singleton(e1);
        }
        else
        {
            e1 = f1;
        }

        T f2 = find(e2);
        if (f2 == notFound)
        {
            singleton(e2);
        }
        else
        {
            e2 = f2;
        }

        if (e1 != e2)
        {
            if (rank[e1] < rank[e2])
            {
                std::swap(e1, e2);
            }
            parent[e2] = e1;
            if (rank[e1] == rank[e2])
            {
                ++rank[e1];
            }
        }
    }

    T find(T e)
    {
    	if (notFound == e || notFound == parent[e])
        {
		    return notFound;
        }

    	if (e == parent[e])
        {
		    return e;
        }

	    return parent[e] = find(parent[e]);
    }

    void print()
    {
        std::cout << "DSU:" << std::endl;
        for (size_t i = 0; i < parent.size(); ++i)
        {
            T f = find(i);
            if (f != notFound)
            {
                std::cout << i << " -> " << f << std::endl;
            }
        }
    }
};
