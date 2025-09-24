#include "OpenCorporaDict.h"

#include <fstream>
#include <filesystem>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <iostream>

#include "../Engine/Utility.h"
#include "../Engine/Printer.h"

#include <spdlog/spdlog.h>

// https://opencorpora.org/dict.php?act=gram
static const std::unordered_map<std::string, std::string> _POSTagTranslator =
{
    {"ADJS", "adj"},
    {"ADJF", "adj"},
    {"ADVB", "adv"},
    {"COMP", "adj"},
    {"CONJ", "sconj"},
    {"GRND", "verb"},
    {"INFN", "adj"},
    {"INTJ", "adj"},
    {"NOUN", "noun"},
    {"NPRO", "pron"},
    {"NUMR", "num"},
    {"PRCL", "part"},
    {"PRED", "adv"},
    {"PREP", "adp"},
    {"PRTF", "verb"},
    {"PRTS", "verb"},
    {"PNCT", "punct"},
    {"SYM",  "sym"},
    {"VERB", "verb"},
};

static const std::unordered_map<std::string, std::initializer_list<std::pair<std::string, std::string>>> _POSTagAdder =
{
    //{"ADJS", {{"variant", "short"}}},
    {"GRND", {{"verbform","ger"}}},
    {"INFN", {{"verbform","inf"}}},
    {"PRTF", {{"verbform","part"}}},
    {"PRTS", {{"verbform","part"},
//              {"variant", "short"}}},
              }},
};

static const std::unordered_map<std::string, std::pair<std::string, std::string>> _FeatureTranslator(
{
    {"Ques", {"prontype", "int"}},
    {"Dmns", {"prontype", "dem"}},

//    {"anim", {"animacy", "anim"}},
//    {"inan", {"animacy", "inan"}},

    {"sing", {"number", "sing"}},
    {"plur", {"number", "plur"}},
    {"Sgtm", {"number", "sing"}},
    {"Pltm", {"number", "plur"}},

    {"Anum", {"numtype", "ord"}},
    {"Coll", {"numtype", "sets"}},

    {"masc", {"gender", "masc"}},
    {"femn", {"gender", "fem"}},
    {"neut", {"gender", "neut"}},

    {"impf", {"aspect", "imp"}},
    {"perf", {"aspect", "perf"}},

    {"pres", {"tense", "pres"}},
    {"past", {"tense", "past"}},
    {"futr", {"tense", "fut"}},

    {"actv", {"voice", "act"}},
    {"pssv", {"voice", "pass"}},

    {"impr", {"mood", "imp"}},
    {"indc", {"mood", "ind"}},

    {"Impe", {"subcat", "indir"}},
    {"Impx", {"subcat", "indir"}},
    {"intr", {"subcat", "intr"}},
    {"tran", {"subcat", "tran"}},

//    {"excl", {"clusivity", "ex"}},
//   {"incl", {"clusivity", "in"}},

    {"nomn", {"case", "nom"}},
    {"gent", {"case", "gen"}},
    {"gen1", {"case", "gen"}},
    {"gen2", {"case", "gen"}},
    {"datv", {"case", "dat"}},
    {"accs", {"case", "acc"}},
    {"acc2", {"case", "acc"}},
    {"ablt", {"case", "abl"}},
    {"loct", {"case", "loc"}},
    {"loc1", {"case", "loc"}},
    {"loc2", {"case", "loc"}},
    {"voct", {"case", "voc"}},

    {"Fixd", {"decl", "zero"}},

    {"1per", {"person", "1"}},
    {"2per", {"person", "2"}},
    {"3per", {"person", "3"}},

    {"Abbr", {"abbr", "yes"}},
    {"Init", {"abbr", "yes"}},
    {"Name", {"nametype", "giv"}},
    {"Surn", {"nametype", "sur"}},
    {"Patr", {"nametype", "pat"}},
    {"Orgn", {"nametype", "com"}},
    {"Trad", {"nametype", "com"}},
    {"Geox", {"nametype", "geo"}},

    {"Supr", {"degree", "sup"}},
    {"Cmp",  {"degree", "cmp"}},
    {"Cmp2", {"degree", "cmp"}},
    {"Poss", {"poss", "yes"}},

//    {"Erro", {"typo", "yes"}},
//    {"Dist", {"style", "vrnc"}},
//    {"Slng", {"style", "slng"}},
//    {"Arch", {"style", "arch"}},
//    {"Infr", {"style", "vrnc"}},
//    {"Litr", {"style", "form"}},
}, 256);

static const std::unordered_set<std::string> _skippedFeatures =
{
// Cannot translate yet
    "Adjx",
    "Af-p",
    "Anph",
    "Apro",
    "Coun",
    "Fimp",
    //"Init",
    "INFN",
    "Inmx",
    "Hypo",
    "Prdx",
    "Prnt",
    "Qual",
    "Subx",
    "V-be", "V-en", "V-ie", "V-bi", "V-sh", "V-oy", "V-ej", "V-ey",
    "Vpre",
    "ms-f", "Ms-f", "GNdr",
    "excl","incl","anim","inan","Erro","Dist","Slng","Arch","Infr","Litr",
};

bool OCDParser::parse(const std::string& fileName, WordsCollection& wc, TagsCollection& tc, DepRelsCollection&, Sentences& sentences)
{
    Printer printer(std::string("Parse ") + fileName, std::filesystem::file_size(fileName));

    std::ifstream stream(fileName, std::fstream::in);

    if (stream.is_open())
    {
        for (std::string line; std::getline(stream, line, '\n');)
        {
            printer.incProgress(line.size());
            if (line.empty() || line.starts_with('#') || line.starts_with('='))
            {
                continue;
            }

            std::string form, rest;
            if (!parsePair(line, "\t", form, rest))
            {
                continue;
            }

            toLower(form);
            filterWord(form);

            std::vector<std::string> wordData = split(rest, "\t ,");

            if (wordData.size() == 0)
            {
                spdlog::debug("Skipped line ({} words) '{}'", wordData.size(), line);
                continue;
            }

            auto posTagTrtd = _POSTagTranslator.find(wordData[0]);
            if (posTagTrtd == _POSTagTranslator.end())
            {
               spdlog::error("Untranslatable POS tag name '{}' in '{}'", wordData[0], line);
               return false;
            }

            POSTag tag;

            tag.POS = tc.POSTag2Index(posTagTrtd->second);
            if(!isValidIndex(tag.POS))
            {
               spdlog::error("Unknown translated POS tag name '{}' in '{}'", posTagTrtd->second, line);
               return false;
            }

            auto POSAdditions = _POSTagAdder.find(wordData[0]);
            if (POSAdditions != _POSTagAdder.end())
            {
                for (const auto& [k, v]: POSAdditions->second)
                {
                    SimpleTagId nameId = tc.featureName2Index(k);
                    SimpleTagId valueId = tc.featureValue2Index(v);

                    tag.features[nameId] = valueId;
                }
            }

            for (size_t i = 1; i < wordData.size(); ++i)
            {
                if (_skippedFeatures.contains(wordData[i]))
                {
                    continue;
                }

                auto featurePair = _FeatureTranslator.find(wordData[i]);
                if (featurePair == _FeatureTranslator.end())
                {
                    spdlog::error("Unknown feature value '{}' in '{}'", wordData[i], line);
                    return false;
                }

                SimpleTagId nameId = tc.featureName2Index(featurePair->second.first);
                SimpleTagId valueId = tc.featureValue2Index(featurePair->second.second);

                if (!isValidIndex(nameId) || !isValidIndex(valueId))
                {
                    spdlog::warn("Skipped feature name/value '{}={}' in '{}'", featurePair->second.first, featurePair->second.second, line);
                    continue;
                }

                tag.features[nameId] = valueId;
            }

            // Artificial sentence with one word only
            Word word;
            word.tags = tc.addTag(tag);
            word.word = wc.addWordForm(word.tags, form);

            Sentence sentence;
            sentence.words.push_back(word);

            sentences.push_back(sentence);
        }
    }

    return true;
}