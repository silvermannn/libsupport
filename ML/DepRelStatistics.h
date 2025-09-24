#pragma once

#include <vector>
#include <optional>
#include <utility>
#include <unordered_map>

#include "../Engine/Sentence.h"
#include "../Collections/DepRelsCollection.h"
#include "../Collections/TagsCollection.h"
#include "../ZLibFile/ZLibFile.h"
#include "../Math/Tensor.h"
#include "../Math/Graph.h"

class DepRelStatistics
{
    typedef Tensor<float, TagId, 3> T;
    typedef Graph<float, TagId> G;

    T stat;

    std::unordered_map<TagId, std::unordered_map<TagId, size_t>> statistics;
public:
    typedef G::Edge Edge;
    typedef G::Edges Edges;

    DepRelStatistics()
        : stat()
    {
    }

    DepRelStatistics(TagId depRelsNum, TagId tagsNum)
        : stat(0, {depRelsNum, tagsNum, tagsNum})
    {
    }

    void resize(TagId depRelsNum, TagId tagsNum)
    {
        stat.resize(0, {depRelsNum, tagsNum, tagsNum});
    }

    void processSentence(const TagsCollection& tc, const DepRelsCollection& drc, const Sentence& sentence);

    void normalize(float smoothingFactor);

    std::optional<Edges> extractGraph(const TagsCollection& tc, const DepRelsCollection& drc, const std::vector<TagId>& tags);

    void saveBinary(ZLibFile& zfile) const;

    bool loadBinary(ZLibFile& zfile);

    void printStatistics(const TagsCollection& tc, const DepRelsCollection& drc) const;
};
