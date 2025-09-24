#include <gtest/gtest.h>

#include "../Collections/WordsCollection.h"

TEST(WordsCollectionTest, Create)
{
    WordsCollection wordsCollection;

    EXPECT_EQ(wordsCollection.wordsSize(), 2);
}


TEST(WordsCollectionTest, WordsAndTags)
{
    const std::string unk("unknownName");
    const std::string word("testWord");
    const std::string wordInitial("testWordInitial");
    const std::string case1("case");
    const std::string acc("acc");

    WordsCollection wordsCollection;

    EXPECT_EQ(wordsCollection.wordsSize(), 2);

}

TEST(WordsCollectionTest, SaveLoadEmpty)
{
    constexpr const char* fileName = "./wordsCollection.bin.gz";

    WordsCollection e1;

    {
        ZLibFile zfile(fileName, true);

        EXPECT_TRUE(zfile.isOpen());

        e1.saveBinary(zfile);
    }

    WordsCollection e2;

    {
        ZLibFile zfile(fileName, false);

        EXPECT_TRUE(zfile.isOpen());

        EXPECT_TRUE(e2.loadBinary(zfile));
    }

    EXPECT_EQ(e1, e2);

    std::remove(fileName);
}
