#include <gtest/gtest.h>

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <cstdio>

#include "../ZLibFile/ZLibFile.h"
#include "Utilites.h"

const char fileName[] = "testFile.gz";

TEST(ZLibFileTest, ReadWriteNumber)
{
    for (size_t t = 0; t < 100; ++t)
    {
        const uint8_t n8 = std::rand();
        const uint16_t n16 = std::rand();
        const uint32_t n32 = std::rand();
        const uint64_t n64 = std::rand();

        {
            ZLibFile zfile(fileName, true);

            EXPECT_TRUE(zfile.isOpen());

            EXPECT_TRUE(zfile.write(n8));
            EXPECT_TRUE(zfile.write(n16));
            EXPECT_TRUE(zfile.write(n32));
            EXPECT_TRUE(zfile.write(n64));
        }

        {
            ZLibFile zfile(fileName, false);

            EXPECT_TRUE(zfile.isOpen());

            uint8_t m8 = std::rand();
            uint16_t m16 = std::rand();
            uint32_t m32 = std::rand();
            uint64_t m64 = std::rand();

            EXPECT_TRUE(zfile.read(m8));
            EXPECT_TRUE(zfile.read(m16));
            EXPECT_TRUE(zfile.read(m32));
            EXPECT_TRUE(zfile.read(m64));

            EXPECT_EQ(n8, m8);
            EXPECT_EQ(n16, m16);
            EXPECT_EQ(n32, m32);
            EXPECT_EQ(n64, m64);
        }

        std::remove(fileName);
    }
}

TEST(ZLibFileTest, ReadWriteString)
{
    for (size_t t = 0; t < 100; ++t)
    {
        std::string s1 = randomString();

        {
            ZLibFile zfile(fileName, true);

            EXPECT_TRUE(zfile.isOpen());

            EXPECT_TRUE(zfile.write(s1));
        }

        {
            std::string s2;

            ZLibFile zfile(fileName, false);

            EXPECT_TRUE(zfile.isOpen());

            EXPECT_TRUE(zfile.read(s2));

            EXPECT_EQ(s1, s2);
        }

        std::remove(fileName);
    }
}

TEST(ZLibFileTest, ReadWriteUnorderedMap)
{
    for (size_t t = 0; t < 100; ++t)
    {
        std::unordered_map<size_t, uint16_t> m1;
        const size_t size = rand() % 1000;

        for (size_t i = 0; i < size; ++i)
        {
            m1[rand()] = rand();
        }

        {
            ZLibFile zfile(fileName, true);

            EXPECT_TRUE(zfile.isOpen());

            EXPECT_TRUE(zfile.write(m1));
        }

        {
            std::unordered_map<size_t, uint16_t> m2;

            ZLibFile zfile(fileName, false);

            EXPECT_TRUE(zfile.isOpen());

            EXPECT_TRUE(zfile.read(m2));

            EXPECT_EQ(m1, m2);
        }

        std::remove(fileName);
    }
}

TEST(ZLibFileTest, ReadWriteUnorderedMapS2S)
{
    for (size_t t = 0; t < 10; ++t)
    {
        std::unordered_map<std::string, std::string> m1;
        const size_t size = rand() % 50;

        for (size_t i = 0; i < size; ++i)
        {
            m1[randomString()] = randomString();
        }

        {
            ZLibFile zfile(fileName, true);

            EXPECT_TRUE(zfile.isOpen());

            EXPECT_TRUE(zfile.write(m1));
        }

        {
            std::unordered_map<std::string, std::string> m2;

            ZLibFile zfile(fileName, false);

            EXPECT_TRUE(zfile.isOpen());

            EXPECT_TRUE(zfile.read(m2));

            EXPECT_EQ(m1, m2);
        }

        std::remove(fileName);
    }
}

TEST(ZLibFileTest, ReadWriteUnorderedSet)
{
    for (size_t t = 0; t < 100; ++t)
    {
        std::unordered_set<uint16_t> s1;
        const size_t size = rand() % 1000;

        for (size_t i = 0; i < size; ++i)
        {
            s1.insert(rand() % 100);
        }

        {
            ZLibFile zfile(fileName, true);

            EXPECT_TRUE(zfile.isOpen());

            EXPECT_TRUE(zfile.write(s1));
        }

        {
            std::unordered_set<uint16_t> s2;

            ZLibFile zfile(fileName, false);

            EXPECT_TRUE(zfile.isOpen());

            EXPECT_TRUE(zfile.read(s2));

            EXPECT_EQ(s1, s2);
        }

        std::remove(fileName);
    }
}
