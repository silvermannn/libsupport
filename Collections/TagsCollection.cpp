#include "TagsCollection.h"

#include <limits>
#include <algorithm>

#include <spdlog/spdlog.h>

#include "BidirectionalMap.h"

static const char defServiceTag[] = "<>";

// https://universaldependencies.org/u/pos/all.html
static const BidirectionalMap<std::string, SimpleTagId> POS_TAGS = {
    defServiceTag,
    "x", "adj", "adp", "adv", "aux", "cconj", "det", "intj", "noun",
    "num", "part", "pron", "propn", "punct", "sconj", "sym", "verb",
};

static const BidirectionalMap<std::string, SimpleTagId> FEATURE_NAMES = {
    "prontype", "gender", "verbform",
    "numtype", "animacy", "mood",
    "poss", "nounclass", "tense",
    "reflex", "number", "aspect",
    "other", "case", "voice",
    "abbr", "definite", "evident",
    "typo", "deixis", "polarity",
    "foreign", "deixisref", "person",
    "extpos", "degree", "polite",
    "clusivity", "numform", "hyph",
    "subcat", "nametype", "style",
    // added for short verbs/adjs
    "variant",
    // added for noun w/o declension
    "decl",
};

static const BidirectionalMap<std::string, SimpleTagId> FEATURE_VALUES = {
    /* prontype  */ "art", "dem", "emp", "exc", "ind", "int", "neg", "prs", "rcp", "rel", "tot",
    /* numtype   */ "card", "dist", "frac", "mult", "ord", "range", "sets",
    /* numform   */ "combi", "digit", "roman", "word",
    /* poss      */ "yes",
    /* reflex    */ "yes",
    /* abbr      */ "yes",
    /* typo      */ "yes",
    /* foreign   */ "yes",
    /* extpos    */ "adj", "adp", "adv", "aux", "cconj", "det", "intj", "pron", "propn", "sconj",
    /* gender    */ "com", "fem", "masc", "neut",
    /* animacy   */ "anim", "hum", "inan", "nhum", "any"
    /* nounclass */ // skipped intentionally
    /* number    */ "coll", "count", "dual", "grpa", "grpl", "inv", "pauc", "plur", "ptan", "stan", "sing", "tri",
    /* case      */ "nom", "gen", "dat", "acc", "abl", "ins", "voc", "par", "loc",
                    "abs", "ben", "cmp", "cns", "equ", "erg", "ess", "com", "lat", "ter", "tra", "cau",
                    "ine", "ill", "ela", "add", "ade", "all", "sup", "spl", "del", "sub", "sbe", "per",
                    "tem", "abe", "dis", "ine", "sbl",
    /* definite  */ "com", "cons", "def", "ind", "spec",
    /* deixis    */ "abv", "bel", "even", "med", "nvis", "prox", "remt",
    /* deixisref */ "1", "2",
    /* degree    */ "abs", "aug", "cmp", "dim", "equ", "pos", "sup",
    /* verbform  */ "conv", "fin", "gdv", "ger", "inf", "part", "sup", "vnoun",
    /* mood      */ "cnd", "imp", "ind", "int", "pot", "sub",
                    "adm", "irr", "jus", "nec", "prp", "qot", "des", "opt",
    /* tense     */ "fut", "imp", "past", "pqp", "pres",
    /* aspect    */ "hab", "imp", "iter", "perf", "prog", "prosp",
    /* voice     */ "act", "mid", "pass", "rcp",
                    "antip", "bfoc", "cau", "dir", "inv", "lfoc",
    /* evident   */ "fh", "nfh",
    /* polarity  */ "neg", "pos",
    /* person    */ "0", "1", "2", "3", "4",
    /* polite    */ "elev", "form", "humb", "infm",
    /* clusivity */ "ex", "in",
    /* subcat    */ "indir", "intr", "tran", "ditr",
    /* nametype  */ "com", "geo", "giv", "nat", "oth", "pat", "pro", "prs", "sur", "zoon",
    /* style     */ "arch", "coll", "expr", "form", "rare", "slng", "vrnc", "vulg",

    /* variant   */ "short",
    /* decl      */ "zero"
};

TagsCollection::TagsCollection()
{
    reset();
}

bool TagsCollection::operator==(const TagsCollection& other) const
{
    return tags == other.tags;
}

void TagsCollection::reset()
{
    tags.clear();

    POSTag t;
    t.POS = POS_TAGS.lookup("x");
    _serviceTag = addTag(t);
    _unknownTag = addTag(t);
}

TagId TagsCollection::tagsSize() const
{
    return tags.size();
}

TagId TagsCollection::serviceTag() const
{
    return _serviceTag;
}

TagId TagsCollection::unknownTag() const
{
    return _unknownTag;
}

TagId TagsCollection::addTag(const POSTag& tag)
{
    return tags.lookupOrInsert(tag);
}

TagId TagsCollection::findMostSimilarTag(const POSTag& tag, const TagSet& tags)
{
    size_t minimalDiff = std::numeric_limits<size_t>::max();
    TagId mostSimilarTag = invalidIndex<TagId>();
    for (const auto& tid: tags)
    {
        const auto& t = getPOSTag(tid);
        if (!t)
        {
            continue;
        }

        size_t diff = *t - tag;
        if (minimalDiff > diff)
        {
            minimalDiff = diff;
            mostSimilarTag = tid;
        }
    }

    if (isValidIndex(mostSimilarTag))
    {
        return mostSimilarTag;
    }

    return invalidIndex<TagId>();
}

std::optional<POSTag> TagsCollection::getPOSTag(TagId tag) const
{
    if (!isValidIndex(tag) || tag >= tags.size())
    {
        spdlog::error("Wrong tag id {}", tag);
        return {};
    }

    return std::make_optional(tags.lookupIndex(tag));
}

SimpleTagId TagsCollection::POSTag2Index(const std::string& s) const
{
    return POS_TAGS.lookup(s);
}

SimpleTagId TagsCollection::featureName2Index(const std::string& s) const
{
    return FEATURE_NAMES.lookup(s);
}

SimpleTagId TagsCollection::featureValue2Index(const std::string& s) const
{
    return FEATURE_VALUES.lookup(s);
}

std::optional<std::string> TagsCollection::index2POSTag(SimpleTagId tag) const
{
    if (!isValidIndex(tag) || tag >= POS_TAGS.size())
    {
        spdlog::error("Wrong POS tag id {}", tag);
        return {};
    }

    return std::make_optional(POS_TAGS.lookupIndex(tag));
}

std::optional<std::string> TagsCollection::index2FeatureName(SimpleTagId tag) const
{
    if (!isValidIndex(tag) || tag >= FEATURE_NAMES.size())
    {
        spdlog::error("Wrong feature name tag id {}", tag);
        return {};
    }

    return std::make_optional(FEATURE_NAMES.lookupIndex(tag));
}

std::optional<std::string> TagsCollection::index2FeatureValue(SimpleTagId tag) const
{
    if (!isValidIndex(tag) || tag >= FEATURE_VALUES.size())
    {
        spdlog::error("Wrong feature value tag id {}", tag);
        return {};
    }

    return std::make_optional(FEATURE_VALUES.lookupIndex(tag));
}

bool TagsCollection::saveBinary(ZLibFile& zfile) const
{
    return tags.saveBinary(zfile);
}

bool TagsCollection::loadBinary(ZLibFile& zfile)
{
    return tags.loadBinary(zfile);
}

void TagsCollection::saveTags(std::ostream& stream)
{
    for(size_t i = 0; i < tags.size(); ++i)
    {
        const auto& t = tags.lookupIndex(i);
        stream << *index2POSTag(t.POS) << " (";
        for (const auto& [k, v]: t.features)
        {
            stream << *index2FeatureName(k) << ":" << *index2FeatureValue(v) << ", ";
        }
        stream << ")" << std::endl;
    }
}
