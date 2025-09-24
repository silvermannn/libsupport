#pragma once

#include "POSTag.h"
#include "../ZLibFile/ZLibFile.h"

struct DepRelTag
{
    SimpleTagId depRel = 0;
    SimpleTagId modifier = 0;
    bool headBefore = false;

    bool operator==(const DepRelTag& other) const
    {
        return depRel == other.depRel && modifier == other.modifier && headBefore == other.headBefore;
    }
};

template <>
struct std::hash<DepRelTag>
{
  std::size_t operator()(const DepRelTag& k) const
  {
    return std::hash<uint64_t>{}((uint64_t)k.depRel ^ (uint64_t)k.modifier << 32) ^ (uint64_t)k.headBefore << 17;
  }
};
