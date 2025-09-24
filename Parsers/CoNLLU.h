#pragma once

#include <string>

#include "Parser.h"
#include "../Collections/WordsCollection.h"
#include "../Collections/TagsCollection.h"
#include "../Collections/DepRelsCollection.h"
#include "../Engine/Sentence.h"

class CoNLLUParser: public Parser
{
public:
    virtual ~CoNLLUParser() {};

    virtual bool parse(const std::string& fileName, WordsCollection& wc, TagsCollection& tc, DepRelsCollection& drc, Sentences& sentences);
};
