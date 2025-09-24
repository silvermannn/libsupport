#pragma once

#include <algorithm>
#include <limits>

#include "../Math/Tensor.h"
#include "../ZLibFile/ZLibFile.h"

#include "spdlog/spdlog.h"

template<typename N, typename HS, typename ES>
class HMM
{
    Tensor<N, HS, 2> hss2hs;
    Tensor<N, ES, 2> hss2es;

public:
    HMM()
        : hss2hs()
        , hss2es()
    {
    };

    HMM(HS hiddenStates, ES emissions)
        : hss2hs(0, {hiddenStates, hiddenStates})
        , hss2es(0, {hiddenStates, emissions})
    {
    };

    ~HMM() {}

    bool operator==(const HMM<N, HS, ES>& other) const
    {
        return hss2hs == other.hss2hs && hss2es == other.hss2es;
    }
    
    void resize(HS hiddenStates, ES emissions)
    {
        spdlog::debug("Resize HMM {} {}", hiddenStates, emissions);
        hss2hs.resize(0, {hiddenStates, hiddenStates});
        hss2es.resize(0, {hiddenStates, emissions});
    }

    void addHiddenState2HiddenState(HS srcHS, HS dstHS)
    {
        ++hss2hs.at(srcHS, dstHS);
    }

    void addHiddenState2Emission(HS srcHS, ES dstES)
    {
        ++hss2es.at(srcHS, dstES);
    }

    void normalize(double smoothingFactor)
    {
        spdlog::debug("Normalizing HMM {}", smoothingFactor);
        hss2hs.normalizeLog(smoothingFactor, 0);
        hss2es.normalizeLog(smoothingFactor, 0);
    }

    std::vector<HS> predict(HS serviceTag, const std::vector<ES>& emissions) const
    {
        spdlog::debug("Predicting by HMM");

        if (emissions.empty())
        {
            spdlog::debug("No input provided");
            return std::vector<HS>();
        }

        const HS hsNum = hss2es.sizeAt(0);
        const size_t seqSize = emissions.size();

        Tensor<N, size_t, 2> prob(-std::numeric_limits<N>::infinity(), {seqSize, hsNum});
        Tensor<N, size_t, 2> prev(0, {seqSize, hsNum});

        std::vector<N> inits(hsNum);
        for (size_t i = 0; i < inits.size(); ++i)
        {
            inits[i] = hss2hs.at(serviceTag, i);
        }

        for(HS hsTo = 0; hsTo < hsNum; hsTo++)
        {
            prob.at(0, hsTo) = inits[hsTo] + hss2es.at(hsTo, emissions[0]);
        }

        for (size_t i = 1; i < seqSize; ++i)
        {
            for(HS hsTo = 0; hsTo < hsNum; hsTo++)
            {
                for(HS hsFrom = 0; hsFrom < hsNum; hsFrom++)
                {
                    N p = prob.at(i - 1, hsFrom) + hss2hs.at(hsFrom, hsTo) + hss2es.at(hsTo, emissions[i]);
                    if (p > prob.at(i, hsTo))
                    {
                        prob.at(i, hsTo) = p;
                        prev.at(i, hsTo) = hsFrom;
                    }
                }
            }
        }

        std::vector<HS> res(seqSize);

        N pMax = -std::numeric_limits<N>::infinity();
        res[emissions.size() - 1] = serviceTag;
        for(HS hsFrom = 0; hsFrom < hsNum; hsFrom++)
        {
            N p = prob.at(emissions.size() - 1, hsFrom) + hss2hs.at(hsFrom, serviceTag);
            if (p > pMax)
            {
                pMax = p;
                res[emissions.size() - 1] = hsFrom;
            }
        }

        for (size_t i = emissions.size() - 1; i != 0; --i)
        {
            res[i - 1] = prev.at(i, res[i]);
        }

        return res;
    };

    void saveBinary(ZLibFile& zfile) const
    {
        hss2hs.saveBinary(zfile);
        hss2es.saveBinary(zfile);
    }

    bool loadBinary(ZLibFile& zfile)
    {
        return hss2hs.loadBinary(zfile)
            && hss2es.loadBinary(zfile);
    }
};
