#pragma once

#include <vector>
#include <unordered_map>
#include <cassert>
#include <cmath>

#include "../ZLibFile/ZLibFile.h"

template <class Index>
const Index& invalidIndex()
{
    static const Index iix = Index(-1);
    return iix;
};

template <class Index>
bool isValidIndex(Index ix)
{
    return ix != invalidIndex<Index>();
};

template <class Item, class Index>
class BidirectionalMap
{
    std::unordered_map<Item, Index> item2index;
    std::vector<const Item*> index2item;

public:
    BidirectionalMap() {};

    BidirectionalMap(const std::initializer_list<Item> items)
    {
        std::for_each(items.begin(), items.end(), [this](auto item) { lookupOrInsert(item); });
    }

    bool operator==(const BidirectionalMap<Item, Index>& other) const
    {
        return item2index == other.item2index && index2item.size() == other.index2item.size();
    }

    void clear(void)
    {
        item2index.clear();
        index2item.clear();
    }

    size_t size(void) const { return index2item.size(); };
    size_t bits(void) const { return std::ceil(std::log2(size())); };

    const Index& lookupOrInsert(const Item& item)
    {
        const auto res = item2index.try_emplace(item, index2item.size());

        if (res.second)
        {
            index2item.push_back(&res.first->first);
        }

        return res.first->second;
    }

    const Index& lookup(const Item& item) const
    {
        auto res = item2index.find(item);
        if (res == item2index.end())
        {
            return invalidIndex<Index>();
        }

        return res->second;
    }

    const Item& lookupIndex(const Index index) const
    {
        if (isValidIndex<Index>(index) && index < index2item.size())
        {
            return *index2item[index];
        }

        assert("Index too big!" && false);
    }

    bool saveBinary(ZLibFile& zfile) const
    {
        return zfile.write(item2index);
    }

    bool loadBinary(ZLibFile& zfile)
    {
        if (!zfile.read(item2index))
        {
            return false;
        }

        index2item.resize(item2index.size());
        for (const auto& [k, v]: item2index)
        {
            index2item[v] = &k;
        }

        return true;
    }
};
