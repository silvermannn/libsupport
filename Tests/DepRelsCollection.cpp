#include <gtest/gtest.h>

#include "../Collections/DepRelsCollection.h"

TEST(DepRelsCollectionTest, Create)
{
    DepRelsCollection depRelsCollection;

    EXPECT_EQ(depRelsCollection.depRelsSize(), 1);
}

TEST(DepRelsCollectionTest, DepRels)
{
    const std::string unk("unknownName");
    const std::string nsubj("nsubj");
    const std::string nummod("nummod");

    DepRelsCollection depRelsCollection;

    EXPECT_EQ(depRelsCollection.depRelsSize(), 1);

    EXPECT_FALSE(isValidIndex(depRelsCollection.dependencyRelation2index(unk)));
    EXPECT_FALSE(isValidIndex(depRelsCollection.dependencyRelationModifier2index(unk)));

    DepRelTag tag;
    tag.depRel = depRelsCollection.dependencyRelation2index(nsubj);
    tag.modifier = depRelsCollection.dependencyRelationModifier2index(nummod);

    EXPECT_TRUE(isValidIndex(tag.depRel));
    EXPECT_TRUE(isValidIndex(tag.modifier));

    TagId id = depRelsCollection.addDepRel(tag);
    EXPECT_EQ(depRelsCollection.depRelsSize(), 2);

    {
        auto res = depRelsCollection.index2dependencyRelation(1000);
        EXPECT_FALSE(res);
    }

    {
        auto res = depRelsCollection.index2dependencyRelationModifier(1000);
        EXPECT_FALSE(res);
    }

    {
        auto res = depRelsCollection.getDependencyRelationTag(id);
        EXPECT_TRUE(res);
        EXPECT_EQ(*res, tag);
    }

    {
        auto res = depRelsCollection.index2dependencyRelation(tag.depRel);
        EXPECT_TRUE(res);
        EXPECT_EQ(*res, nsubj);
    }

    {
        auto res = depRelsCollection.index2dependencyRelationModifier(tag.modifier);
        EXPECT_TRUE(res);
        EXPECT_EQ(*res, nummod);
    }
}

TEST(DepRelsCollectionTest, SaveLoadEmpty)
{
    constexpr const char* fileName = "./depRelsCollection.bin.gz";

    DepRelsCollection e1;

    {
        ZLibFile zfile(fileName, true);

        EXPECT_TRUE(zfile.isOpen());

        e1.saveBinary(zfile);
    }

    DepRelsCollection e2;

    {
        ZLibFile zfile(fileName, false);

        EXPECT_TRUE(zfile.isOpen());

        EXPECT_TRUE(e2.loadBinary(zfile));
    }

    EXPECT_EQ(e1, e2);

    std::remove(fileName);
}

TEST(DepRelsCollectionTest, SaveLoad)
{
    constexpr const char* fileName = "./depRelsCollection.bin.gz";
    const std::string noun("noun");
    const std::string case1("case");
    const std::string acc("acc");
    const std::string nsubj("nsubj");
    const std::string nummod("nummod");

    DepRelsCollection e1;

    DepRelTag drtag;
    drtag.depRel = e1.dependencyRelation2index(nsubj);
    drtag.modifier = e1.dependencyRelationModifier2index(nummod);
    e1.addDepRel(drtag);

    {
        ZLibFile zfile(fileName, true);

        EXPECT_TRUE(zfile.isOpen());

        e1.saveBinary(zfile);
    }

    DepRelsCollection e2;

    {
        ZLibFile zfile(fileName, false);

        EXPECT_TRUE(zfile.isOpen());

        EXPECT_TRUE(e2.loadBinary(zfile));
    }

    EXPECT_EQ(e1, e1);
    EXPECT_EQ(e1, e2);

    std::remove(fileName);
}