#pragma once

#include <unordered_map>
#include <unordered_set>

#include <x86intrin.h>

#include "../ZLibFile/ZLibFile.h"

typedef uint8_t SimpleTagId;
typedef uint16_t TagId;

struct POSTag
{
    SimpleTagId POS = 0;
    std::unordered_map<SimpleTagId, SimpleTagId> features;

    bool operator==(const POSTag& other) const
    {
        return POS == other.POS && features == other.features;
    }

    size_t operator-(const auto& other) const
    {
        if (&other == this)
        {
            return 0;
        }

        size_t diff = (POS != other.POS)?100:0;
        size_t maxSize = std::max(features.size(), other.features.size());
        size_t numberOfSimilar = std::count_if(features.begin(), features.end(), [&](const auto& pair)
        {
            const auto& it = other.features.find(pair.first);
            return it != other.features.end() && it->second == pair.second;
        });

        return diff + maxSize - numberOfSimilar;
    }
};

typedef std::unordered_set<TagId> TagSet;

template<>
bool ZLibFile::write<POSTag>(const POSTag& s);

template<>
bool ZLibFile::read<POSTag>(POSTag& s);

template <>
struct std::hash<POSTag>
{
  std::size_t operator()(const POSTag& k) const
  {
    uint64_t res = (uint64_t)k.POS;
    for (const auto& [k, v]: k.features)
    {
        res ^= uint64_t(k);
        _rotl(res, 8);
        res ^= uint64_t(v);
        _rotl(res, 8);
    }
    return std::hash<uint64_t>{}(res);
  }
};
