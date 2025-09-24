#pragma once

#include <vector>

#include "../Collections/POSTag.h"
#include "../Collections/WordsCollection.h"
#include "../ZLibFile/ZLibFile.h"

struct Word
{
    WordId word = 0;
    WordId initialWord = 0;
    TagId tags = 0;
    size_t depHead = -1;
    TagId depRel = 0;

    bool operator==(const Word& other) const
    {
        return word == other.word &&
               initialWord == other.initialWord &&
               tags == other.tags &&
               depHead == other.depHead &&
               depRel == other.depRel;
    }
};

struct Sentence
{
    std::vector<Word> words;

    void saveBinary(ZLibFile& zfile) const;
    bool loadBinary(ZLibFile& zfile);

    bool operator==(const Sentence& other) const
    {
        return words == other.words;
    }
};

typedef std::vector<Sentence> Sentences;
