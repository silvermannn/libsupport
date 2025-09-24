#include <gtest/gtest.h>

#include "../Collections/TagsCollection.h"

TEST(TagsCollectionTest, Create)
{
    TagsCollection tagsCollection;

    EXPECT_EQ(tagsCollection.tagsSize(), 1);
}

TEST(TagsCollectionTest, Tags)
{
    const std::string unk("unknownName");
    const std::string noun("noun");
    const std::string case1("case");
    const std::string acc("acc");

    TagsCollection tagsCollection;

    EXPECT_EQ(tagsCollection.tagsSize(), 1);

    EXPECT_FALSE(isValidIndex(tagsCollection.POSTag2Index(unk)));
    EXPECT_FALSE(isValidIndex(tagsCollection.featureName2Index(unk)));
    EXPECT_FALSE(isValidIndex(tagsCollection.featureValue2Index(unk)));

    POSTag tag;
    tag.POS = tagsCollection.POSTag2Index(noun);
    tag.features[tagsCollection.featureName2Index(case1)] = tagsCollection.featureValue2Index(acc);

    EXPECT_TRUE(isValidIndex(tag.POS));
    EXPECT_EQ(tag.features.size(), 1);
    for (const auto& [k, v]: tag.features)
    {
        EXPECT_TRUE(isValidIndex(k));
        EXPECT_TRUE(isValidIndex(v));
    }

    TagId id = tagsCollection.addTag(tag);
    EXPECT_EQ(tagsCollection.tagsSize(), 2);

    {
        auto res = tagsCollection.index2POSTag(1000);
        EXPECT_FALSE(res);
    }

    {
        auto res = tagsCollection.getPOSTag(id);
        EXPECT_TRUE(res);
        EXPECT_EQ(*res, tag);
    }

    {
        auto res = tagsCollection.index2POSTag(tag.POS);
        EXPECT_TRUE(res);
        EXPECT_EQ(*res, noun);
    }

    for (const auto& [k, v]: tag.features)
    {
        {
            auto res = tagsCollection.index2FeatureName(k);
            EXPECT_TRUE(res);
            EXPECT_EQ(*res, case1);
        }
        {
            auto res = tagsCollection.index2FeatureValue(v);
            EXPECT_TRUE(res);
            EXPECT_EQ(*res, acc);
        }
    }
}

TEST(TagsCollectionTest, SaveLoadEmpty)
{
    constexpr const char* fileName = "./tagsCollection.bin.gz";

    TagsCollection e1;

    {
        ZLibFile zfile(fileName, true);

        EXPECT_TRUE(zfile.isOpen());

        e1.saveBinary(zfile);
    }

    TagsCollection e2;

    {
        ZLibFile zfile(fileName, false);

        EXPECT_TRUE(zfile.isOpen());

        EXPECT_TRUE(e2.loadBinary(zfile));
    }

    EXPECT_EQ(e1, e2);

    std::remove(fileName);
}
