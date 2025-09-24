#pragma once

#include <string>
#include <vector>
#include <optional>
#include <unordered_map>
#include <unordered_set>

#include "POSTag.h"
#include "BidirectionalMap.h"
#include "../ZLibFile/ZLibFile.h"

typedef uint32_t WordId;

class WordsCollection
{
    BidirectionalMap<std::string, WordId> _words;

    std::unordered_map<WordId, TagSet> _wids2tags;

    WordId _serviceWord;
    WordId _unknownWord;

public:
    WordsCollection();

    bool operator==(const WordsCollection& other) const;

    void reset();

    size_t wordsSize() const;
    std::pair<WordId, size_t> maxTagsPerWord() const;

    WordId addWord(const std::string& word);
    WordId addWordForm(TagId tagId, const std::string& word);

    WordId serviceWord() const;
    WordId unknownWord() const;

    WordId word2index(const std::string& word) const;
    std::optional<std::string> index2word(WordId word);

    const TagSet& findTagsForWord(WordId word) const;

    bool saveBinary(ZLibFile& zfile) const;
    bool loadBinary(ZLibFile& zfile);
};
