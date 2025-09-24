#pragma once

#include <string>
#include <vector>
#include <optional>
#include <unordered_map>

#include "BidirectionalMap.h"
#include "DepRelTag.h"
#include "../Engine/Sentence.h"

class DepRelsCollection
{
    BidirectionalMap<DepRelTag, TagId> depRelTags;

    TagId _depRelRoot;

public:
    DepRelsCollection();

    bool operator==(const DepRelsCollection& other) const;

    void reset();

    TagId depRelsSize() const;

    TagId depRelRoot() const;

    TagId addDepRel(const DepRelTag& dr);

    std::optional<DepRelTag> getDependencyRelationTag(TagId tag) const;

    SimpleTagId dependencyRelation2index(const std::string& s) const;
    std::optional<std::string> index2dependencyRelation(SimpleTagId tag) const;

    SimpleTagId dependencyRelationModifier2index(const std::string& s) const;
    std::optional<std::string> index2dependencyRelationModifier(SimpleTagId tag) const;

    bool saveBinary(ZLibFile& zfile) const;

    bool loadBinary(ZLibFile& zfile);
};
