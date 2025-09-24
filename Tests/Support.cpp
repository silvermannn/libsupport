#include <gtest/gtest.h>

#include <fstream>

#include "../Support.h"
#include "../Parsers/Parser.h"
#include "../Collections/WordsCollection.h"
#include "../Collections/TagsCollection.h"
#include "../Collections/DepRelsCollection.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wwrite-strings"

static const char TestCoNLLU[] = " \n\
# sent_id = n01118003 \n\
# text = Drop the mic. \n\
1	Drop	drop	VERB	VB	VerbForm=Inf	0	root	0:root	_\n \
2	the	the	DET	DT	Definite=Def|PronType=Art	3	det	3:det	_ \n\
3	mic	mic	NOUN	NN	Number=Sing	1	obj	1:obj	SpaceAfter=No \n\
4	.	.	PUNCT	.	_	1	punct	1:punct	_ \n\
";

TEST(SupportCInterfaceTest, ParseCoNLLU)
{
    constexpr char* fileName = "./test.conllu";

    {
        std::ofstream test(fileName);
        test << TestCoNLLU;
        test.close();
    }

    EXPECT_TRUE(parse(fileName, "CoNLLU"));

    std::remove(fileName);
}

struct FakeParser: public Parser
{
    bool success = false;

    virtual bool parse(const std::string&, WordsCollection&, TagsCollection&, DepRelsCollection&, Sentences&)
    {
        return success;
    };

    FakeParser() {};
    virtual ~FakeParser() {};
};

TEST(SupportCInterfaceTest, RegisterAndUseParser)
{
    constexpr char* parserName = "Fake";
    constexpr char* fileName = "./test.fake";

    {
        std::ofstream test(fileName);
    }

    FakeParser fakeParser;

    EXPECT_TRUE(registerParser(parserName, &fakeParser));

    EXPECT_FALSE(registerParser(parserName, &fakeParser));

    EXPECT_FALSE(parse(fileName, parserName));

    fakeParser.success = true;

    EXPECT_TRUE(parse(fileName, parserName));

    EXPECT_TRUE(unregisterParser(parserName));

    EXPECT_FALSE(unregisterParser(parserName));

    std::remove(fileName);
}

TEST(SupportCInterfaceTest, LoadNonExisting)
{
    constexpr char* fileName = "./.nonexisting.file";

    std::remove(fileName);

    EXPECT_FALSE(parse(fileName, "CoNLLU"));

    EXPECT_FALSE(parse(fileName, "RANDOM_PARSER"));

    std::remove(fileName);
}

TEST(SupportCInterfaceTest, SaveLoad)
{
    constexpr char* fileName = "./test.conllu";
    constexpr char* nativeFileName = "./test.bin.gz";

    {
        std::ofstream test(fileName);
        test << TestCoNLLU;
        test.close();
    }

    EXPECT_TRUE(parse(fileName, "CoNLLU"));

    EXPECT_TRUE(saveSentences(nativeFileName));

    EXPECT_TRUE(loadSentences(nativeFileName));

    EXPECT_TRUE(saveEncoder(nativeFileName));

    EXPECT_TRUE(loadEncoder(nativeFileName));

    std::remove(fileName);
    std::remove(nativeFileName);
}

TEST(SupportCInterfaceTest, TrainTagger)
{
    constexpr char* fileName = "./test.conllu";
    constexpr char* nativeFileName = "./test.bin.gz";

    {
        std::ofstream test(fileName);
        test << TestCoNLLU;
        test.close();
    }

    EXPECT_TRUE(parse(fileName, "CoNLLU"));

    EXPECT_TRUE(trainTagger(0.5));

    EXPECT_TRUE(saveTagger(nativeFileName));

    EXPECT_TRUE(loadTagger(nativeFileName));

    std::remove(fileName);
    std::remove(nativeFileName);
}


TEST(SupportCInterfaceTest, TrainTreeBuilder)
{
    constexpr char* fileName = "./test.conllu";
    constexpr char* nativeFileName = "./test.bin.gz";

    {
        std::ofstream test(fileName);
        test << TestCoNLLU;
        test.close();
    }

    EXPECT_TRUE(parse(fileName, "CoNLLU"));

    EXPECT_TRUE(trainTreeBuilder(0.5));

    EXPECT_TRUE(saveTreeBuilder(nativeFileName));

    EXPECT_TRUE(loadTreeBuilder(nativeFileName));

    std::remove(fileName);
    std::remove(nativeFileName);
}

#pragma GCC diagnostic pop
