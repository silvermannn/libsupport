#pragma once

#include <vector>
#include <optional>
#include <iostream>

#include "POSTag.h"
#include "../ZLibFile/ZLibFile.h"
#include "BidirectionalMap.h"

class TagsCollection
{
    BidirectionalMap<POSTag, TagId> tags;

    TagId _serviceTag;
    TagId _unknownTag;

public:
    TagsCollection();

    bool operator==(const TagsCollection& other) const;

    void reset();

    TagId tagsSize() const;

    TagId addTag(const POSTag& tag);
    TagId findMostSimilarTag(const POSTag& tag, const TagSet& tags);

    std::optional<POSTag> getPOSTag(TagId tag) const;

    TagId serviceTag() const;
    TagId unknownTag() const;

    std::optional<std::string> index2POSTag(SimpleTagId tag) const;
    SimpleTagId POSTag2Index(const std::string& s) const;

    std::optional<std::string> index2FeatureName(SimpleTagId tag) const;
    SimpleTagId featureName2Index(const std::string& s) const;

    std::optional<std::string> index2FeatureValue(SimpleTagId tag) const;
    SimpleTagId featureValue2Index(const std::string& s) const;

    bool saveBinary(ZLibFile& zfile) const;
    bool loadBinary(ZLibFile& zfile);

    void saveTags(std::ostream& stream);
};
