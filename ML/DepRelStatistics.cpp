#include "DepRelStatistics.h"

#include <fstream>
#include <cmath>

#include "spdlog/spdlog.h"

#include "../Math/MSTD.h"

void DepRelStatistics::processSentence(const TagsCollection& tc, const DepRelsCollection& drc, const Sentence& sentence)
{
    size_t zeroCount = 0;
    for (const auto& word: sentence.words)
    {
        if (word.depHead == 0)
            ++zeroCount;
        if (zeroCount > 2)
            return;
    }

    for (size_t i = 0; i < sentence.words.size(); ++i)
    {
        const auto& word = sentence.words[i];
        if (!isValidIndex(word.depHead))
        {
            continue;
        }
        if (word.depHead > sentence.words.size())
        {
            continue;
        }
        TagId src = word.depHead == 0 ? drc.depRelRoot(): sentence.words[word.depHead - 1].tags;
        TagId dest = word.tags;
        ++stat.at(word.depRel, src, dest);

        (statistics[dest][word.depRel])++;
    }
}

void DepRelStatistics::normalize(float smoothingFactor)
{
    spdlog::debug("Normalizing tree builder statistics {}", smoothingFactor);
    stat.normalizeLog(smoothingFactor, 2);
}

std::optional<DepRelStatistics::Edges> DepRelStatistics::extractGraph(const TagsCollection& tc, const DepRelsCollection& drc, const std::vector<TagId>& tags)
{
    spdlog::debug("Extracting tree from graph for {} tags, {} labels, root {}", tags.size(), stat.sizeAt(0), drc.depRelRoot());

    DepRelStatistics::G g(tags.size() + 1, stat.sizeAt(0));

    for (TagId depRel = 0; depRel < stat.sizeAt(0); ++depRel)
    {
        auto drTag = drc.getDependencyRelationTag(depRel);
        if (!drTag)
        {
            continue;
        }
        for (TagId i1 = 0; i1 < tags.size(); ++i1)
        {
            TagId src = tags[i1];

            g.addEdge(0, i1 + 1, depRel, stat.at(depRel, 0, src) - std::log(tags.size() + i1));

            for (TagId i2 = 0; i2 < tags.size(); ++i2)
            {
                TagId dest = tags[i2];

                if (i1 == i2)
                    continue;

                if (drTag->headBefore != (i1 < i2))
                {
                    continue;
                }

                float distancePenalty = std::log(std::fabs(float(i1) - float(i2)));

                g.addEdge(i1 + 1, i2 + 1, depRel, stat.at(depRel, src, dest) - distancePenalty);
            }
        }
    }

    {
        std::ofstream s("dr-src.dot");
        g.saveDot(s);
    }

    ChuLiuEdmondsMST solver(g);

    auto p = solver.getSpanningTree(0);

    {
        std::ofstream s("dr.dot");
        g.saveDot(s);
    }

    return p;
}

void DepRelStatistics::saveBinary(ZLibFile& zfile) const
{
    stat.saveBinary(zfile);
}

bool DepRelStatistics::loadBinary(ZLibFile& zfile)
{
    return stat.loadBinary(zfile);
}

void DepRelStatistics::printStatistics(const TagsCollection& tc, const DepRelsCollection& drc) const
{
    std::ofstream stream("dr.csv");
    stream << "tags\t" << statistics.size() << std::endl;

    stream << "TAG/RELNAME" ;
    for (size_t dr = 0; dr < drc.depRelsSize(); ++dr)
    {
        auto drTag = drc.getDependencyRelationTag(dr);
        if (!drTag)
        {
            continue;
        }

        auto drName = drc.index2dependencyRelation(drTag->depRel);
        if (!drName)
        {
            continue;
        }

        stream << "\t" << dr << ":" << *drName;

        auto drMod = drc.index2dependencyRelationModifier(drTag->modifier);

        if (drMod)
        {
            stream << ":" << *drMod;
        }

        stream << (drTag->headBefore?"->":"<-");

    }

    stream << std::endl;

    for (const auto& [t, m]: statistics)
    {
        auto ct = tc.getPOSTag(t);
        if (ct)
        {
            auto tn = tc.index2POSTag(ct->POS);
            stream << *tn ;
        }
        for (size_t dr = 0; dr < drc.depRelsSize(); ++dr)
        {
            auto v = m.find(dr);
            if (v == m.end())
                stream << "\t 0";
            else
                stream << "\t" << v->second;
        }
        stream << std::endl;
    }
}
