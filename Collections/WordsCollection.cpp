#include "WordsCollection.h"

WordsCollection::WordsCollection()
{
    reset();
}

bool WordsCollection::operator==(const WordsCollection& other) const
{
    return _words == other._words && _wids2tags == other._wids2tags;
}

void WordsCollection::reset()
{
    _words.clear();
    _wids2tags.clear();

    _serviceWord = addWord("<>");
    _unknownWord = addWord("<unknown>");
}

WordId WordsCollection::addWord(const std::string& word)
{
    return _words.lookupOrInsert(word);
}

WordId WordsCollection::addWordForm(TagId tagId, const std::string& word)
{
    if (!isValidIndex(tagId))
    {
        return invalidIndex<WordId>();
    }

    WordId id = _words.lookupOrInsert(word);

    _wids2tags[id].insert(tagId);

    return id;
}

WordId WordsCollection::serviceWord() const
{
    return _serviceWord;
}

WordId WordsCollection::unknownWord() const
{
    return _unknownWord;
}

size_t WordsCollection::wordsSize() const
{
    return _words.size();
}

std::pair<WordId, size_t> WordsCollection::maxTagsPerWord() const
{
    size_t smax = 0;
    WordId w = invalidIndex<WordId>();
    for(size_t i = 0; i < _words.size(); ++i)
    {
        size_t s = findTagsForWord(i).size();
        if (s > smax)
        {
            smax = s;
            w = i;
        }
    }
    
    return {w, smax};
}

WordId WordsCollection::word2index(const std::string& word) const
{
    WordId w = _words.lookup(word);
    if (!isValidIndex(w))
    {
        return _unknownWord;
    }
    
    return w;
}

std::optional<std::string> WordsCollection::index2word(WordId word)
{
    if (!isValidIndex(word) || word > _words.size())
    {
        return {};
    }

    return std::make_optional(_words.lookupIndex(word));
}

const TagSet& WordsCollection::findTagsForWord(WordId word) const
{
    const auto& tags = _wids2tags.find(word);

    if (tags == _wids2tags.end())
    {
        static TagSet empty;
        return empty;
    }
    
    return tags->second;
}

bool WordsCollection::saveBinary(ZLibFile& zfile) const
{
    return _words.saveBinary(zfile) && zfile.write(_wids2tags);
}

bool WordsCollection::loadBinary(ZLibFile& zfile)
{
    return _words.loadBinary(zfile) && zfile.read(_wids2tags);
}
