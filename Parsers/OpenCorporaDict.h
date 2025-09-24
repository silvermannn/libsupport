#pragma once

#include "Parser.h"
#include "../Collections/WordsCollection.h"
#include "../Collections/TagsCollection.h"
#include "../Collections/DepRelsCollection.h"
#include "../Engine/Sentence.h"

class OCDParser: public Parser
{
public:
    virtual ~OCDParser() {};

    virtual bool parse(const std::string& fileName, WordsCollection& wc, TagsCollection& tc, DepRelsCollection& drc, Sentences& sentences);
};
