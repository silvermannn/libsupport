#include <gtest/gtest.h>

#include <cmath>
#include <cstdio>

#include "../Math/Tensor.h"
#include "../ZLibFile/ZLibFile.h"

TEST(TensorTest, CreateEmpty)
{
    constexpr size_t arity = 2;

    Tensor<int, size_t, arity> t;

    EXPECT_EQ(t.size(), 0);

    for (size_t i = 0; i < arity; ++i)
    {
        EXPECT_EQ(t.sizeAt(i), 0);
    }
}

TEST(TensorTest, SetGet)
{
    constexpr size_t arity = 2;
    constexpr size_t x = 31415;
    constexpr size_t y = 42;

    Tensor<int, uint16_t, arity> t(0.0f, {x, y});

    EXPECT_EQ(t.size(), x * y);
    EXPECT_EQ(t.sizeAt(0), x);
    EXPECT_EQ(t.sizeAt(1), y);

    for (size_t i = 0; i < 100; ++i)
    {
        for (size_t j = 0; j < x; ++j)
        {
            for (size_t k = 0; k < y; ++k)
            {
                t.at(j,k) = (k+1)*(j+1)*(i+1);
            }
        }

        for (size_t j = 0; j < x; ++j)
        {
            for (size_t k = 0; k < y; ++k)
            {
                EXPECT_EQ(t.at(j,k), (k+1)*(j+1)*(i+1));
            }
        }
    }
}

TEST(TensorTest, Resize2D)
{
    constexpr size_t arity = 2;

    Tensor<int, uint16_t, arity> t(0, {0, 0});

    EXPECT_EQ(t.size(), 0);
    EXPECT_EQ(t.sizeAt(0), 0);
    EXPECT_EQ(t.sizeAt(1), 0);
    
    for(size_t i = 0; i < 10; ++i)
    {
        const uint16_t x = 1 + std::rand() % 5000;
        const uint16_t y = 1 + std::rand() % 5000;

        t.resize(0, {x, y});

        EXPECT_EQ(t.size(), x * y);
        EXPECT_EQ(t.sizeAt(0), x);
        EXPECT_EQ(t.sizeAt(1), y);

        for (uint16_t j = 0; j < x; ++j)
        {
            for (uint16_t k = 0; k < y; ++k)
            {
                t.at(j,k) = (k+1)*(j+1)*(i+1);
            }
        }

        for (uint16_t j = 0; j < x; ++j)
        {
            for (uint16_t k = 0; k < y; ++k)
            {
                EXPECT_EQ(t.at(j,k), (k+1)*(j+1)*(i+1));
            }
        }
    }
}

TEST(TensorTest, Resize3D)
{
    constexpr size_t arity = 3;

    Tensor<int, uint16_t, arity> t(0, {0, 0, 0});

    EXPECT_EQ(t.size(), 0);
    EXPECT_EQ(t.sizeAt(0), 0);
    EXPECT_EQ(t.sizeAt(1), 0);
    EXPECT_EQ(t.sizeAt(2), 0);
    
    for(size_t i = 0; i < 5; ++i)
    {
        const uint16_t x = 1 + std::rand() % 500;
        const uint16_t y = 1 + std::rand() % 500;
        const uint16_t z = 1 + std::rand() % 500;

        t.resize(0, {x, y, z});

        EXPECT_EQ(t.size(), x * y * z);
        EXPECT_EQ(t.sizeAt(0), x);
        EXPECT_EQ(t.sizeAt(1), y);
        EXPECT_EQ(t.sizeAt(2), z);

        for (uint16_t j = 0; j < x; ++j)
        {
            for (uint16_t k = 0; k < y; ++k)
            {
                for (uint16_t n = 0; n < z; ++n)
                {
                    t.at(j,k,n) = (k+1)*(j+1)*(i+1)*n;
                }
            }
        }

        for (uint16_t j = 0; j < x; ++j)
        {
            for (uint16_t k = 0; k < y; ++k)
            {
                for (uint16_t n = 0; n < z; ++n)
                {
                    EXPECT_EQ(t.at(j,k,n), (k+1)*(j+1)*(i+1)*n);
                }
            }
        }
    }
}

TEST(TensorTest, Resize4D)
{
    constexpr size_t arity = 4;

    Tensor<int, uint8_t, arity> t(0, {0, 0, 0, 0});

    EXPECT_EQ(t.size(), 0);
    EXPECT_EQ(t.sizeAt(0), 0);
    EXPECT_EQ(t.sizeAt(1), 0);
    EXPECT_EQ(t.sizeAt(2), 0);
    EXPECT_EQ(t.sizeAt(3), 0);
    
    for(size_t i = 0; i < 10; ++i)
    {
        const uint8_t x = 1 + std::rand() % 50;
        const uint8_t y = 1 + std::rand() % 50;
        const uint8_t z = 1 + std::rand() % 50;
        const uint8_t w = 1 + std::rand() % 50;

        t.resize(0, {x, y, z, w});

        EXPECT_EQ(t.size(), x * y * z * w);
        EXPECT_EQ(t.sizeAt(0), x);
        EXPECT_EQ(t.sizeAt(1), y);
        EXPECT_EQ(t.sizeAt(2), z);
        EXPECT_EQ(t.sizeAt(3), w);

        for (uint8_t j = 0; j < x; ++j)
        {
            for (uint8_t k = 0; k < y; ++k)
            {
                for (uint8_t n = 0; n < z; ++n)
                {
                    for (uint8_t m = 0; m < w; ++m)
                    {
                        t.at(j,k,n,m) = (k+1)*(j+1)*(i+1)*n-m;
                    }
                }
            }
        }

        for (uint8_t j = 0; j < x; ++j)
        {
            for (uint8_t k = 0; k < y; ++k)
            {
                for (uint8_t n = 0; n < z; ++n)
                {
                    for (uint8_t m = 0; m < w; ++m)
                    {
                        EXPECT_EQ(t.at(j,k,n,m), (k+1)*(j+1)*(i+1)*n-m);
                    }
                }
            }
        }
    }
}

TEST(TensorTest, SaveLoad)
{
    constexpr const char* fileName = "./tensor.bin.gz";
    constexpr size_t arity = 2;

    for(size_t t = 0; t < 100; ++t)
    {
        const size_t x = 10 + std::rand() % 100;
        const size_t y = 10 + std::rand() % 100;

        Tensor<int, size_t, arity> t1(0, {x, y});

        for (size_t j = 0; j < x; ++j)
        {
            for (size_t k = 0; k < y; ++k)
            {
                t1.at(j,k) = std::rand();
            }
        }

        {
            ZLibFile zfile(fileName, true);

            EXPECT_TRUE(zfile.isOpen());

            t1.saveBinary(zfile);
        }

        Tensor<int, size_t, arity> t2;

        {
            ZLibFile zfile(fileName, false);

            EXPECT_TRUE(zfile.isOpen());

            EXPECT_TRUE(t2.loadBinary(zfile));
        }

        EXPECT_EQ(t1, t2);

    }

    std::remove(fileName);
}

TEST(TensorTest, Normalize2D)
{
    constexpr size_t arity = 2;
    constexpr size_t size = 1337;

    for (float iv = 0.0; iv < 10.0; iv += 3)
    {
        for (float sf = 0; sf <= 1.0; sf += 0.5)
        {
            const float sum = iv * size + sf * size;

            for (size_t i = 0; i < arity; ++i)
            {
                Tensor<float, size_t, arity> t(iv, {size, size});

                EXPECT_EQ(t.size(), size * size);

                if (iv == 0.0 && sf == 0.0)
                {
                    continue;
                }

                t.normalize(sf, i);

                for (size_t j = 0; j < size; ++j)
                {
                    for (size_t k = 0; k < size; ++k)
                    {
                        EXPECT_FLOAT_EQ(t.at(j,k), (iv + sf)/sum);
                    }
                }
            }
        }
    }
}

TEST(TensorTest, NormalizeLog2D)
{
    constexpr size_t arity = 2;
    constexpr size_t size = 1337;

    for (float iv = 0.0; iv < 10.0; iv += 3)
    {
        for (float sf = 0; sf <= 1.0; sf += 0.5)
        {
            const float sum = log(iv * size + sf * size);

            for (size_t i = 0; i < arity; ++i)
            {
                Tensor<float, size_t, arity> t(iv, {size, size});

                EXPECT_EQ(t.size(), size * size);

                if (iv == 0.0 && sf == 0.0)
                {
                    continue;
                }

                t.normalizeLog(sf, i);

                for (size_t j = 0; j < size; ++j)
                {
                    for (size_t k = 0; k < size; ++k)
                    {
                        EXPECT_FLOAT_EQ(t.at(j,k), log(iv + sf) - sum);
                    }
                }
            }
        }
    }
}

TEST(TensorTest, Normalize3D)
{
    constexpr size_t arity = 3;
    constexpr size_t size = 71;

    for (float iv = 0.0; iv < 10.0; iv += 3)
    {
        for (float sf = 0; sf <= 1.0; sf += 0.5)
        {
            const float sum = iv * size * size + sf * size * size;

            for (size_t i = 0; i < arity; ++i)
            {
                Tensor<float, size_t, arity> t(iv, {size, size, size});

                EXPECT_EQ(t.size(), size * size * size);

                if (iv == 0.0 && sf == 0.0)
                {
                    continue;
                }

                t.normalize(sf, i);

                for (size_t j = 0; j < size; ++j)
                {
                    for (size_t k = 0; k < size; ++k)
                    {
                        for (size_t l = 0; l < size; ++l)
                        {
                            EXPECT_FLOAT_EQ(t.at(j,k,l), (iv + sf)/sum);
                        }
                    }
                }
            }
        }
    }
}

TEST(TensorTest, NormalizeLog3D)
{
    constexpr size_t arity = 3;
    constexpr size_t size = 71;

    for (float iv = 0.0; iv < 10.0; iv += 3)
    {
        for (float sf = 0; sf <= 1.0; sf += 0.5)
        {
            const float sum = log(iv * size * size + sf * size * size);

            for (size_t i = 0; i < arity; ++i)
            {
                Tensor<float, size_t, arity> t(iv, {size, size, size});

                EXPECT_EQ(t.size(), size * size * size);

                if (iv == 0.0 && sf == 0.0)
                {
                    continue;
                }

                t.normalizeLog(sf, i);

                for (size_t j = 0; j < size; ++j)
                {
                    for (size_t k = 0; k < size; ++k)
                    {
                        for (size_t l = 0; l < size; ++l)
                        {
                            EXPECT_FLOAT_EQ(t.at(j,k,l), log(iv + sf) - sum);
                        }
                    }
                }
            }
        }
    }
}
