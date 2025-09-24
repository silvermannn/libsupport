#pragma once

#include <cstddef>
#include <vector>
#include <functional>
#include <numeric>
#include <cmath>
#include <iostream>

#include "../ZLibFile/ZLibFile.h"

#include "spdlog/spdlog.h"

// N-dimensional matirx
template<typename N, typename IndexT, size_t Arity>
class Tensor
{
    struct Normalizer
    {
        N sum;
        N smoothingFactor;

        Normalizer(N s, IndexT size, N sf)
            : sum(s + sf * size)
            , smoothingFactor(sf)
        {
        }

        N operator()(N arg) const
        {
            return (arg + smoothingFactor) / sum;
        }
    };

    struct NormalizerLog
    {
        N sum;
        N smoothingFactor;

        NormalizerLog(N s, IndexT size, N sf)
            : sum(log(s + sf * size))
            , smoothingFactor(sf)
        {
        }

        N operator()(N arg) const
        {
            return log(arg + smoothingFactor) - sum;
        }
    };

    std::vector<IndexT> sizes;

    N* data = nullptr;
    N* sums = nullptr;
    IndexT sumsSize = 0;

    template<typename T, typename...Rest>
    uint64_t getOffset(IndexT sizeIdx, T ix, Rest... indexes) const
    {
        return ix + getOffset(sizeIdx + 1, indexes...) * sizes[sizeIdx];
    }

    template<typename T>
    uint64_t getOffset(IndexT, T ix) const
    {
        return ix;
    }

    void allocate(N initialValue)
    {
        spdlog::debug("Allocate {} elements for Tensor", size());
        data = new N[size()];
        std::fill(data, data + size(), initialValue);
    }

    void free()
    {
        spdlog::debug("Free Tensor");

        delete data;
        data = nullptr;

        delete sums;
        sums = nullptr;
    }

    N getAt(IndexT ix, IndexT i) const
    {
        if (ix == 0)
        {
            return getAt(ix, i, i);
        }

        N s = 0;
        for (IndexT j = 0; j < sizes[0]; ++j)
        {
            s += getAt(ix, i, j);
        }

        return s;
    }

    template<typename...Ints>
    requires (sizeof...(Ints) < Arity)
    N getAt(IndexT ix, IndexT i, Ints... indexes) const
    {
        if (ix == sizeof...(indexes))
        {
            return getAt(ix, i, indexes..., i);
        }

        N s = 0;
        for (IndexT j = 0; j < sizes[sizeof...(indexes)]; ++j)
        {
            s += getAt(ix, i, indexes..., j);
        }

        return s;
    }

    template<typename...Ints>
    requires (sizeof...(Ints) == Arity)
    N getAt(IndexT, IndexT, Ints... indexes) const
    {
        return at(indexes...);
    }

    void calculateSums(IndexT ix)
    {
        delete sums;

        sumsSize = sizes[ix];
        sums = new N[sumsSize];
        std::fill(sums, sums + sumsSize, 0);
        for (IndexT i = 0; i < sumsSize; ++i)
        {
            sums[i] = getAt(ix, i);
        }
    }

    template<typename Norm>
    void normalizeAt(Norm norm, N s, IndexT ix, IndexT i)
    {
        if (ix == 0)
        {
            return normalizeAt(norm, s, ix, i, i);
        }

        for (IndexT j = 0; j < sizes[0]; ++j)
        {
            normalizeAt(norm, s, ix, i, j);
        }
    }

    template<typename Norm, typename...Ints>
    requires (sizeof...(Ints) < Arity)
    void normalizeAt(Norm norm, N s, IndexT ix, IndexT i, Ints... indexes)
    {
        if (ix == sizeof...(indexes))
        {
            return normalizeAt(norm, s, ix, i, indexes..., i);
        }

        for (IndexT j = 0; j < sizes[sizeof...(indexes)]; ++j)
        {
            normalizeAt(norm, s, ix, i, indexes..., j);
        }
    }

    template<typename Norm, typename...Ints>
    requires (sizeof...(Ints) == Arity)
    void normalizeAt(Norm norm, N, IndexT, IndexT, Ints... indexes)
    {
        N& v = at(indexes...);
        v = norm(v);
    }

    template<typename Norm>
    void normalizeImpl(N smoothingFactor, IndexT ix)
    {
        calculateSums(ix);
        for (IndexT i = 0; i < sumsSize; ++i)
        {
            Norm norm(sums[i], N(size() / sumsSize), smoothingFactor);
            normalizeAt(norm, sums[i], ix, i);
        }
    }


public:
    Tensor()
        : sizes(Arity, 0)
    {
    }

    Tensor(N _initialValue, const IndexT(&_sizes)[Arity])
        : sizes(std::begin(_sizes), std::end(_sizes))
    {
        allocate(_initialValue);
    }

    ~Tensor()
    {
        free();
    }

    void resize(N _initialValue, const IndexT(&_sizes)[Arity])
    {
        spdlog::debug("Resize Tensor {}", Arity);
        free();
        sizes = std::vector<IndexT>(std::begin(_sizes), std::end(_sizes));
        allocate(_initialValue);
    }

    bool operator==(const Tensor<N, IndexT, Arity>& other) const
    {
        return sizes == other.sizes && memcmp(data, other.data, size() * sizeof(N)) == 0;
    }

    IndexT sizeAt(IndexT dim) const
    {
        return sizes[dim];
    }

    uint64_t size() const
    {
        return std::accumulate(sizes.begin(), sizes.end(), uint64_t(1), std::multiplies<uint64_t>());
    }

    template<typename... Ints>
    const N& at(Ints... indexes) const
    {
        static_assert(sizeof...(indexes) == Arity);
        uint64_t offset = getOffset(0, indexes...);
        return data[offset];
    }

    template<typename... Ints>
    N& at(Ints... indexes)
    {
        static_assert(sizeof...(indexes) == Arity);
        uint64_t offset = getOffset(0, indexes...);
        return data[offset];
    }

    void normalize(N smoothingFactor, IndexT ix)
    {
        return normalizeImpl<Normalizer>(smoothingFactor, ix);
    }

    void normalizeLog(N smoothingFactor, IndexT ix)
    {
        return normalizeImpl<NormalizerLog>(smoothingFactor, ix);
    }

    void saveBinary(ZLibFile& zfile) const
    {
        zfile.write(uint8_t(Arity));
        for (IndexT i = 0; i < sizes.size(); ++i)
        {
            zfile.write(sizes[i]);
        }

        zfile.writePtr(data, size());
    }

    bool loadBinary(ZLibFile& zfile)
    {
        uint8_t arity = 0;
        zfile.read(arity);

        if (arity != Arity)
        {
            return false;
        }

        free();

        sizes.resize(Arity);
        for (IndexT i = 0; i < sizes.size(); ++i)
        {
            if (!zfile.read(sizes[i]) || sizes[i] == 0)
            {
                return false;
            }
        }

        allocate(0);

        return zfile.readPtr(data, size());
    }

    void print() const
    {
        for (IndexT i = 0; i < size(); ++i)
        {
            std::cout << data[i] << ", ";
        }
        std::cout << std::endl;

        if (sums)
        {
            for (IndexT i = 0; i < sumsSize; ++i)
            {
                std::cout << N(sums[i]) << ", ";
            }
        }
        std::cout << std::endl << std::endl;
   }
};
