#include "Support.h"

#include <iostream>

#include "Engine/Engine.h"

#include "spdlog/spdlog.h"

const size_t MAX_NAME_LENGTH = 64;
const size_t MAX_FEATURES_PER_WORD = 16;
const size_t MAX_BUFFER_SIZE = (2 * MAX_FEATURES_PER_WORD + 1) * MAX_NAME_LENGTH;

static char buffer[MAX_BUFFER_SIZE];

bool registerParser(char* parserName, void* parser)
{
    return Engine::singleton().registerParser(parserName, *(Parser*)parser);
}

bool unregisterParser(char* parserName)
{
    return Engine::singleton().unregisterParser(parserName);
}

bool parse(char* path, char* parserName)
{
    return Engine::singleton().parse(path, parserName);
}

bool trainTagger(float smoothingFactor)
{
    return Engine::singleton().trainTagger(smoothingFactor);
}

bool trainTreeBuilder(float smoothingFactor)
{
    return Engine::singleton().trainTreeBuilder(smoothingFactor);
}

bool saveSentences(char* path)
{
    return Engine::singleton().saveSentences(path);
}

bool loadSentences(char* path)
{
    return Engine::singleton().loadSentences(path);
}

bool saveEncoder(char* path)
{
    return Engine::singleton().saveCollections(path);
}

bool loadEncoder(char* path)
{
    return Engine::singleton().loadCollections(path);
}

bool saveTreeBuilder(char* path)
{
    return Engine::singleton().saveTreeBuilder(path);
}

bool loadTreeBuilder(char* path)
{
    return Engine::singleton().loadTreeBuilder(path);
}

bool saveTagger(char* path)
{
    return Engine::singleton().saveTagger(path);
}

bool loadTagger(char* path)
{
    return Engine::singleton().loadTagger(path);
}

bool index2word(size_t w, char** result)
{
    if (!result)
    {
        spdlog::error("Result is null");
        return false;
    }

    const auto s = Engine::singleton().getWordsCollection().index2word(w);

    if (!s)
    {
        spdlog::error("Failed to get word for index {}", w);
        return false;
    }

    result[0] = buffer;
    stpncpy(result[0], s->c_str(), MAX_NAME_LENGTH);

    return true;
}

bool word2index(char* word, size_t* result)
{
    if (!result)
    {
        spdlog::error("Result is null");
        return false;
    }

    result[0] = Engine::singleton().getWordsCollection().word2index(word);

    return true;
}

bool tag(size_t* words, size_t len, size_t* result)
{
    if (!result)
    {
        spdlog::error("Result is null");
        return false;
    }

    Words v(len);
    std::copy(words, words + len, v.begin());

    std::optional<Tags> res = Engine::singleton().tag(v);

    if (!res)
    {
        return false;
    }

    std::copy(res->begin(), res->end(), result);

    return true;
}

bool getCompoundPOSTag(size_t tag, size_t* result, size_t* len)
{
    if (!result || !len)
    {
        spdlog::error("Result is null");
        return false;
    }

    const auto cpt = Engine::singleton().getTagsCollection().getPOSTag(tag);

    if (!cpt)
    {
        spdlog::error("failed to get compound tag for tag {}", tag);
        return false;
    }

    *len = 1;
    result[0] = cpt->POS;

    for (const auto& [k, v]: cpt->features)
    {
        result[*len] = k;
        result[*len + 1] = v;
        *len += 2;
    }

    return true;
}

bool index2POSTag(size_t tag, char** result)
{
    if (!result)
    {
        spdlog::error("Result is null");
        return false;
    }

    const auto s = Engine::singleton().getTagsCollection().index2POSTag(tag);

    if (!s)
    {
        spdlog::error("failed to get POS name for tag {}", tag);
        return false;
    }

    result[0] = buffer;
    stpncpy(result[0], s->c_str(), MAX_NAME_LENGTH);

    return true;
}

bool index2FeatureName(size_t tag, char** result)
{
    if (!result)
    {
        spdlog::error("Result is null");
        return false;
    }

    const auto s = Engine::singleton().getTagsCollection().index2FeatureName(tag);

    if (!s)
    {
        spdlog::error("failed to get feature name for tag {}", tag);
        return false;
    }

    result[0] = buffer;
    stpncpy(result[0], s->c_str(), MAX_NAME_LENGTH);

    return true;
}

bool index2FeatureValue(size_t tag, char** result)
{
    if (!result)
    {
        spdlog::error("Result is null");
        return false;
    }

    const auto s = Engine::singleton().getTagsCollection().index2FeatureValue(tag);

    if (!s)
    {
        spdlog::error("failed to get feature value name for tag {}", tag);
        return false;
    }

    result[0] = buffer;
    stpncpy(result[0], s->c_str(), MAX_NAME_LENGTH);

    return true;
}

bool buildDependencyTree(size_t* tags, size_t len, size_t* result)
{
    if (!result)
    {
        spdlog::error("Result is null");
        return false;
    }

    std::vector<TagId> t(len);
    std::copy(tags, tags + len, t.begin());

    std::optional<DepRelStatistics::Edges> res = Engine::singleton().buildDependencyTree(t);

    if (!res)
    {
        return false;
    }

    for (size_t i = 0; i < len; ++i)
    {
        result[3 * i] = (*res)[i].src;
        result[3 * i + 1] = (*res)[i].dest;
        result[3 * i + 2] = (*res)[i].label;
    }

    return true;
}

bool getCompoundDeprelTag(size_t tag, size_t* result, size_t* len)
{
    if (!result || !len)
    {
        spdlog::error("Result is null");
        return false;
    }

    const auto cpt = Engine::singleton().getDepRelsCollection().getDependencyRelationTag(tag);

    if (!cpt)
    {
        spdlog::error("Failed to get compound dependency relation tag for tag {}", tag);
        return false;
    }

    *len = 3;
    result[0] = cpt->depRel;
    result[1] = cpt->modifier;
    result[2] = cpt->headBefore;

    return true;
}

bool index2dependencyRelation(size_t tag, char** result)
{
    if (!result)
    {
        spdlog::error("Result is null");
        return false;
    }

    const auto s = Engine::singleton().getDepRelsCollection().index2dependencyRelation(tag);

    if (!s)
    {
        spdlog::error("Failed to get description for tag {}", tag);
        return false;
    }

    result[0] = buffer;
    stpncpy(result[0], s->c_str(), MAX_NAME_LENGTH);

    return true;
}

bool index2dependencyRelationModifier(size_t tag, char** result)
{
    if (!result)
    {
        spdlog::error("Result is null");
        return false;
    }

    const auto s = Engine::singleton().getDepRelsCollection().index2dependencyRelationModifier(tag);

    if (!s)
    {
        spdlog::error("Failed to get description for tag {}", tag);
        return false;
    }

    result[0] = buffer;
    stpncpy(result[0], s->c_str(), MAX_NAME_LENGTH);

    return true;
}
