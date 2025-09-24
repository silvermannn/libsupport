#include "DepRelsCollection.h"

#include <algorithm>
#include <iterator>

#include "spdlog/spdlog.h"

static const BidirectionalMap<std::string, SimpleTagId> DEP_RELS = {
    /* Core arguments      */ "nsubj", "obj", "iobj", "csubj", "ccomp", "xcomp",
    /* Non-core dependents */ "obl", "vocative", "expl", "dislocated", "advcl",
                              "advmod", "discourse", "aux", "cop", "mark",
    /* Nominal dependents  */ "nmod", "appos", "nummod", "acl", "amod", "det", "clf", "case",
    /* Coordination        */ "conj", "cc",
    /* Headless            */ "fixed", "flat",
    /* Loose               */ "list", "parataxis",
    /* Special             */ "compound", "orphan", "goeswith", "reparandum",
    /* Other               */ "punct", "root", "dep",
};

static const BidirectionalMap<std::string, SimpleTagId> DEP_RELS_MODIFIERS = {
    "", "outer", "pass", "agent", "arg", "lmod", "tmod", "outer", "pass", "emph", "lmod", "impers", "pass", "relcl", "poss",
    "pass", "tmod", "numgov", "nummod", "gov", "foreign", "name", "lvc", "prt", "redup", "svc", "pv", "relcl", "poss", "preconj",
    "entity", "discourse",
};

DepRelsCollection::DepRelsCollection()
{
    reset();
}

bool DepRelsCollection::operator==(const DepRelsCollection& other) const
{
    return depRelTags == other.depRelTags;
}

void DepRelsCollection::reset()
{
    depRelTags.clear();

    DepRelTag root;
    root.depRel = dependencyRelation2index("root");
    root.modifier = dependencyRelationModifier2index("");
    _depRelRoot = addDepRel(root);
}

TagId DepRelsCollection::depRelRoot() const
{
    return _depRelRoot;
}

TagId DepRelsCollection::depRelsSize() const
{
    return depRelTags.size();
}

TagId DepRelsCollection::addDepRel(const DepRelTag& dr)
{
    return depRelTags.lookupOrInsert(dr);
}

SimpleTagId DepRelsCollection::dependencyRelation2index(const std::string& s) const
{
    return DEP_RELS.lookup(s);
}

std::optional<std::string> DepRelsCollection::index2dependencyRelation(SimpleTagId tag) const
{
    if (!isValidIndex(tag) || tag >= DEP_RELS.size())
    {
        spdlog::error("Wrong dependency relation tag id {}", tag);
        return {};
    }

    return std::make_optional(DEP_RELS.lookupIndex(tag));
}

SimpleTagId DepRelsCollection::dependencyRelationModifier2index(const std::string& s) const
{
    return DEP_RELS_MODIFIERS.lookup(s);
}

std::optional<std::string> DepRelsCollection::index2dependencyRelationModifier(SimpleTagId tag) const
{
    if (!isValidIndex(tag) || tag >= DEP_RELS_MODIFIERS.size())
    {
        spdlog::error("Wrong dependency relation modifier tag id {}", tag);
        return {};
    }

    return std::make_optional(DEP_RELS_MODIFIERS.lookupIndex(tag));
}

std::optional<DepRelTag> DepRelsCollection::getDependencyRelationTag(TagId tag) const
{
    if (!isValidIndex(tag) || tag >= depRelTags.size())
    {
        spdlog::error("Wrong dependency relation compound tag id {}", tag);
        return {};
    }

    return std::make_optional(depRelTags.lookupIndex(tag));
}

bool DepRelsCollection::saveBinary(ZLibFile& zfile) const
{
    return depRelTags.saveBinary(zfile);
}

bool DepRelsCollection::loadBinary(ZLibFile& zfile)
{
    return depRelTags.loadBinary(zfile);
}
