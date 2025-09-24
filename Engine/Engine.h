#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <optional>

#include "../ML/HMM.h"
#include "../ML/DepRelStatistics.h"
#include "../Collections/WordsCollection.h"
#include "../Collections/TagsCollection.h"
#include "../Collections/DepRelsCollection.h"
#include "../Parsers/Parser.h"
#include "Sentence.h"
#include "Printer.h"

typedef std::vector<std::string> Strings;
typedef std::vector<TagId> Tags;
typedef std::vector<WordId> Words;

class Engine
{
    std::unordered_map<std::string, Parser&> parsers;

    std::vector<Sentence> sentences;

    WordsCollection wordsCollection;
    TagsCollection tagsCollection;
    DepRelsCollection depRelsCollection;

    HMM<float, TagId, WordId> hmm;
    DepRelStatistics drStat;

    Sentence unkWordOnly;

    bool parseDirectory(const std::string& path, const std::string& parserName);

    bool parseFile(const std::string& path, const std::string& parserName);

    void trainHMMOnSentence(const Sentence& sentence);

public:
    static Engine& singleton();

    Engine();

    void reset(void);

    std::vector<std::string> availableParsers() const;

    bool registerParser(const std::string& parserName, Parser& parser);

    bool unregisterParser(const std::string& parserName);

    void clearSentences();

    bool trainTagger(float smoothingFactor);

    bool trainTreeBuilder(double smoothingFactor);

    bool parse(const std::string& path, const std::string& parserName);

    bool saveSentences(const std::string& fileName) const;

    bool loadSentences(const std::string& fileName);

    bool saveCollections(const std::string& fileName) const;

    bool loadCollections(const std::string& fileName);

    std::optional<Tags> tag(const Words& sentence) const;

    bool saveTagger(const std::string& fileName) const;

    bool loadTagger(const std::string& fileName);

    bool saveTreeBuilder(const std::string& fileName) const;

    bool loadTreeBuilder(const std::string& fileName);

    std::optional<DepRelStatistics::Edges> buildDependencyTree(const std::vector<TagId>& tags);

    WordsCollection& getWordsCollection();
    TagsCollection& getTagsCollection();
    DepRelsCollection& getDepRelsCollection();
};
