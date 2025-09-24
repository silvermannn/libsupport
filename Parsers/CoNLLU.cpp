#include "CoNLLU.h"

#include <sstream>
#include <fstream>
#include <filesystem>

#include "../Engine/Utility.h"
#include "../Engine/Printer.h"

#include "spdlog/spdlog.h"

std::string fixTag(const std::string& s)
{
    if (s == "h") { return "adv"; }
    else if (s == "conj") { return "cconj"; }
    else return s;
}

bool fixFeatureName(std::string& s, const std::string& oldpos, std::string& pos)
{
    if (s.starts_with("form"))
    {
        s = "numform";
        return true;
    }
    if (s.starts_with("tran"))
    {
        s = "subcat";
        return true;
    }
    if (s.starts_with("anim"))
    {
        s = "animacy";
        return true;
    }
    if ((oldpos == "adj" || oldpos == "noun") && (s == "aspect" || s == "verbform" || s == "subcat" || s == "voice"))
    {
        pos = "verb";
        return false;
    }
    if (oldpos == "adj" && s == "nametype")
    {
        pos = "propn";
        return false;
    }
    if (s.starts_with("predic"))
    {
        // skip
        return false;
    }
    return true;
}

bool fixFeatureValue(std::string& s)
{
    if (s.starts_with("aor") || s.starts_with("notpast"))
    {
        s = "pres";
        return true;
    }
    if (s.starts_with("fut"))
    {
        s = "fut";
        return true;
    }
    if (s.starts_with("tran"))
    {
        s = "tran";
        return true;
    }
    if (s.starts_with("intr"))
    {
        s = "intr";
        return true;
    }
    if (s.starts_with("split") || s.starts_with("init"))
    {
        s = "yes";
        return true;
    }
    if (s.starts_with("ptr"))
    {
        s = "pat";
        return true;
    }
    if (s.starts_with("obsc"))
    {
        s = "vulg";
        return true;
    }
    if (s == "no" || s == "full" || s == "long")
    {
        return false;
    }
    return true;
}

bool CoNLLUParser::parse(const std::string& fileName, WordsCollection& wc, TagsCollection& tc, DepRelsCollection& drc, Sentences& sentences)
{
    Printer printer(std::string("Parse ") + fileName, std::filesystem::file_size(fileName));

    std::ifstream stream(fileName, std::fstream::in);

    if (stream.is_open())
    {
        Sentence sentence;
        size_t lines = 0;
        for (std::string line; std::getline(stream, line, '\n');)
        {
            printer.incProgress(line.size());
            ++lines;
            if (line.empty() || line.starts_with('#') || line.starts_with('='))
            {
                if (!sentence.words.empty())
                {
                    sentences.push_back(sentence);
                    sentence.words.clear();
                }
                continue;
            }

            toLower(line);

            if (line.starts_with('\t'))
            {
                line = '0' + line;
            }

            std::vector<std::string> wordData = split(line, "\t");

            if (wordData.size() > 3)
            {
                Word word;

                filterWord(wordData[1]);
                word.word = wc.word2index(wordData[1]);
                if (!isValidIndex(word.word))
                {
                    spdlog::warn("Unknown word '{}'", wordData[1], line);
                    word.word = wc.unknownWord();
                }

                filterWord(wordData[2]);
                word.initialWord = wc.word2index(wordData[2]);
                if (word.word != wc.unknownWord() && !isValidIndex(word.initialWord))
                {
                    spdlog::warn("Unknown word initial form '{}'", wordData[2]);
                    word.initialWord = wc.unknownWord();
                }

                TagSet tags = wc.findTagsForWord(word.word);
                if (word.word != wc.unknownWord() && tags.empty())
                {
                    spdlog::warn("Not found tags for '{}'", line);
                }

                if (tags.size() == 1)
                {
                    word.tags = *tags.begin();
                }
                else
                {
                    POSTag tag;

                    tag.POS = tc.POSTag2Index(fixTag(wordData[3]));
                    if (!isValidIndex(tag.POS))
                    {
                        spdlog::warn("Unknown POS tag: '{}' in '{}'", wordData[3], line);
                        continue;
                    }

                    std::string featuresLine;
                    if (wordData.size() > 4 && wordData[4] != "_") featuresLine += wordData[4];
                    if (wordData.size() > 5 && wordData[5] != "_") featuresLine += '|' + wordData[5];

                    std::vector<std::string> features = split(featuresLine, "/|");
                    std::sort(features.begin(), features.end());
                    for (auto featurePair: features)
                    {
                        std::string name, value;
                        if (!parsePair(featurePair, "=", name, value))
                        {
                            continue;
                        }

                        std::string newPOS;
                        if (name.empty() || value.empty() || !fixFeatureName(name, wordData[3], newPOS)|| !fixFeatureValue(value))
                        {
                            if (!newPOS.empty())
                            {
                                tag.POS = tc.POSTag2Index(fixTag(wordData[3]));
                                break;
                            }
                            continue;
                        }

                        SimpleTagId fname = tc.featureName2Index(name);
                        SimpleTagId fvalue = tc.featureValue2Index(value);
                        if (isValidIndex(fname) && isValidIndex(fvalue))
                        {
                            tag.features[fname] = fvalue;
                        }
                        else
                        {
                            spdlog::warn("Wrong feature pair '{}={}'", name, value);
                        }
                    }

                    if (word.word != wc.unknownWord())
                    {
                        word.tags = tc.findMostSimilarTag(tag, tags);
                        if (!isValidIndex(word.tags))
                        {
                            spdlog::warn("Not found tag for '{}'", line);
                        }
                    }
                    else
                    {
                        word.tags = tc.unknownTag();
                    }
                }

                try
                {
                    if (wordData.size() > 6) word.depHead = std::stoul(wordData[6]);
                }
                catch(std::invalid_argument&)
                {
                    word.depHead = -1;
                }

                if (isValidIndex(word.depHead) && wordData.size() > 7 && wordData[7] != "_")
                {
                    DepRelTag dr;
                    dr.headBefore = word.depHead <= sentence.words.size();
                    std::string depRelMain, depRelMod;
                    if (!parsePair(wordData[7], ":", depRelMain, depRelMod))
                    {
                        depRelMain = wordData[7];
                        depRelMod = "";
                    }

                    dr.depRel = drc.dependencyRelation2index(depRelMain);
                    if (!isValidIndex(dr.depRel))
                    {
                        spdlog::warn("Unknown dependency relation '{}' for POS tag '{}' in '{}'", depRelMain, wordData[3], wordData[7]);
                    }

                    dr.modifier = drc.dependencyRelationModifier2index(depRelMod);
                    if (!isValidIndex(dr.modifier))
                    {
                        spdlog::warn("Unknown dependency relation modifier '{} : {}' for POS tag '{}' in '{}'", depRelMain, depRelMod, wordData[3], wordData[7]);
                    }

                    word.depRel = drc.addDepRel(dr);
                }
                else
                {
                    word.depHead = -1;
                }

                sentence.words.push_back(word);
            }
        }

        if (!sentence.words.empty())
        {
            sentences.push_back(sentence);
            sentence.words.clear();
        }
    }
    else
    {
        spdlog::error("Failed to open {}", fileName);
        return false;
    }

    return true;
}
