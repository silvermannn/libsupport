#include <string>
#include <vector>

const char defServiceTag[] = "<>";

const std::vector<std::string> EMPTY_FEATURES = {};
const std::vector<std::string> X_FEATURES =     {"typo", "hyph", "abbr", "foreign", };
const std::vector<std::string> ADJ_FEATURES =   {"typo", "hyph", "style", "abbr", "anom", "degree", "gender",
                                                 "number", "case", "numtype", "numform", "variant", "decl", };
const std::vector<std::string> ADP_FEATURES =   {"typo", "hyph", "style", "abbr", "anom", };
const std::vector<std::string> ADV_FEATURES =   {"typo", "hyph", "style", "abbr", "anom", "polarity", "degree", "numtype", "numform", "variant", };
const std::vector<std::string> AUX_FEATURES =   {"typo", "hyph", "gender", "verbform", "mood", "tense", "number", "case", "aspect", "voice", "person", };
const std::vector<std::string> CCONJ_FEATURES = {"typo", "hyph", "polarity", };
const std::vector<std::string> DET_FEATURES =   {"typo", "hyph", "abbr", "anom", "gender", "number", "case", "person", "variant", };
const std::vector<std::string> INTJ_FEATURES =  {"typo", "hyph", "anom", "foreign", };
const std::vector<std::string> NOUN_FEATURES =  {"typo", "hyph", "style", "abbr", "anom", "gender", "animacy",
                                                 "number", "case", "nametype", "numtype", "numform", "decl", };
const std::vector<std::string> NUM_FEATURES =   {"typo", "hyph", "abbr", "numtype", "gender", "number", "case", "numform", };
const std::vector<std::string> PART_FEATURES =  {"typo", "hyph", "style", "anom", "polarity", "mood", };
const std::vector<std::string> PRON_FEATURES =  {"typo", "hyph", "style", "abbr", "anom", "prontype", "polarity", "reflex", "gender", "number", "case", "person", };
const std::vector<std::string> PROPN_FEATURES = {"typo", "hyph", "style", "abbr", "anom", "foreign", "nametype", "gender", "animacy", "number", "case", "numtype", };
const std::vector<std::string> PUNCT_FEATURES = EMPTY_FEATURES;
const std::vector<std::string> SCONJ_FEATURES = {"typo", "hyph", "polarity", "mood", };
const std::vector<std::string> SYM_FEATURES =   {"numtype", };
const std::vector<std::string> VERB_FEATURES =  {"typo", "hyph", "style", "abbr", "anom", "degree", "gender", "verbform", "mood", "tense",
                                                 "number", "case", "aspect", "voice", "person", "subcat", "variant", };

const std::vector<std::string> X_SIMPLE_FEATURES =     EMPTY_FEATURES;
const std::vector<std::string> ADJ_SIMPLE_EATURES =    {"degree", "gender", "number", "case", "numtype", "numform", };
const std::vector<std::string> ADP_SIMPLE_FEATURES =   EMPTY_FEATURES;
const std::vector<std::string> ADV_SIMPLE_FEATURES =   EMPTY_FEATURES;
const std::vector<std::string> AUX_SIMPLE_FEATURES =   {"gender", "number", "case", "voice", "person", };
const std::vector<std::string> CCONJ_SIMPLE_FEATURES = EMPTY_FEATURES;
const std::vector<std::string> DET_SIMPLE_FEATURES =   {"gender", "number", "case", "person", "variant", };
const std::vector<std::string> INTJ_SIMPLE_FEATURES =  EMPTY_FEATURES;
const std::vector<std::string> NOUN_SIMPLE_FEATURES =  {"gender", "number", "case", };
const std::vector<std::string> NUM_SIMPLE_FEATURES =   {"gender", "number", "case", };
const std::vector<std::string> PART_SIMPLE_FEATURES =  {"polarity", };
const std::vector<std::string> PRON_SIMPLE_FEATURES =  {"prontype", "reflex", "gender", "number", "case", "person", };
const std::vector<std::string> PROPN_SIMPLE_FEATURES = {"number", "case", "numtype", };
const std::vector<std::string> PUNCT_SIMPLE_FEATURES = EMPTY_FEATURES;
const std::vector<std::string> SCONJ_SIMPLE_FEATURES = EMPTY_FEATURES;
const std::vector<std::string> SYM_SIMPLE_FEATURES =   EMPTY_FEATURES;
const std::vector<std::string> VERB_SIMPLE_FEATURES =  {"gender", "number", "case", "voice", "person", "subcat", };

struct TagDescription
{
    std::string name;
    std::vector<std::string> items;
    std::vector<std::string> simpleItems;
};

const std::vector<TagDescription> TAG_DESCRIPTIONS = {
    TagDescription {defServiceTag, EMPTY_FEATURES, EMPTY_FEATURES},     // Service
    // https://universaldependencies.org/u/pos/all.html
    TagDescription {"x",     X_FEATURES,     X_SIMPLE_FEATURES},     // other/url/foreign/unknown
    TagDescription {"adj",   ADJ_FEATURES,   ADJ_SIMPLE_EATURES},    // adjective
    TagDescription {"adp",   ADP_FEATURES,   ADP_SIMPLE_FEATURES},   // adposition (prepositions and postpositions)
    TagDescription {"adv",   ADV_FEATURES,   ADV_SIMPLE_FEATURES},   // adverb
    TagDescription {"aux",   AUX_FEATURES,   AUX_SIMPLE_FEATURES},   // auxiliary
    TagDescription {"cconj", CCONJ_FEATURES, CCONJ_SIMPLE_FEATURES}, // coordinating conjunction
    TagDescription {"det",   DET_FEATURES,   DET_SIMPLE_FEATURES},   // determiner
    TagDescription {"intj",  INTJ_FEATURES,  INTJ_SIMPLE_FEATURES},  // interjection
    TagDescription {"noun",  NOUN_FEATURES,  NOUN_SIMPLE_FEATURES},  // noun
    TagDescription {"num",   NUM_FEATURES,   NUM_SIMPLE_FEATURES},   // numeral
    TagDescription {"part",  PART_FEATURES,  PART_SIMPLE_FEATURES},  // particle
    TagDescription {"pron",  PRON_FEATURES,  PRON_SIMPLE_FEATURES},  // pronoun
    TagDescription {"propn", PROPN_FEATURES, PROPN_SIMPLE_FEATURES}, // proper noun
    TagDescription {"punct", PUNCT_FEATURES, PUNCT_SIMPLE_FEATURES}, // punctuation
    TagDescription {"sconj", SCONJ_FEATURES, SCONJ_SIMPLE_FEATURES}, // subordinating conjunction
    TagDescription {"sym",   SYM_FEATURES,   SYM_SIMPLE_FEATURES},   // symbol
    TagDescription {"verb",  VERB_FEATURES,  VERB_SIMPLE_FEATURES},  // verb
};
