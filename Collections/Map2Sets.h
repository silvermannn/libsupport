#pragma once

#include <unordered_map>
#include <unordered_set>
#include <iostream>

#include "spdlog/spdlog.h"

template <class K, class V>
class Map2Sets
{
    std::unordered_map<K, std::unordered_set<V>> mapCheck;
    std::unordered_map<K, std::unordered_set<V>> mapSimple;

public:
    template<typename Initializer>
    Map2Sets(const std::vector<Initializer> initializers)
    {
        std::for_each(initializers.begin(), initializers.end(), [this](auto item)
        {
            mapCheck[item.name];
            std::for_each(item.items.begin(), item.items.end(), [&](auto subitem) {
                mapCheck[item.name].insert(subitem);
            });

            mapSimple[item.name];
            std::for_each(item.simpleItems.begin(), item.simpleItems.end(), [&](auto subitem) {
                mapSimple[item.name].insert(subitem);
            });
        });
    }

    bool operator==(const Map2Sets<K, V>& other) const
    {
        return mapCheck == other.mapCheck && mapSimple == other.mapSimple;
    }

    bool check(const K& k, const V& v) const
    {
        auto it = mapCheck.find(k);
        if (it == mapCheck.end())
        {
            spdlog::info("Key 1 not found: {}", k);
            return false;
        }

        return it->second.contains(v);
    }

    bool checkIsSimple(const K& k, const V& v) const
    {
        auto it = mapSimple.find(k);
        if (it == mapSimple.end())
        {
            spdlog::info("Key 2 not found: {}", k);
            return false;
        }

        return it->second.contains(v);
    }

    void print(void) const
    {
        for (auto [k, v]: mapCheck)
        {
            std::cout << k << ":" << std::endl;
            for (auto i: v)
            {
                std::cout << i << ", ";
            }
            std::cout << std::endl;
        }
    }
};
