#ifndef SUPPORT_C_INTERFACE
#define SUPPORT_C_INTERFACE

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

bool registerParser(char* parserName, void* parser);

bool unregisterParser(char* parserName);

bool parse(char* path, char* parserName);

bool trainTagger(float smoothingFactor);

bool trainTreeBuilder(float smoothingFactor);

bool saveTagger(char* path);

bool loadTagger(char* path);

bool saveSentences(char* path);

bool loadSentences(char* path);

bool saveEncoder(char* path);

bool loadEncoder(char* path);

bool saveTreeBuilder(char* path);

bool loadTreeBuilder(char* path);


bool index2word(size_t w, char** result);

bool word2index(char* word, size_t* result);

    
bool tag(size_t* words, size_t len, size_t* result);

bool getCompoundPOSTag(size_t tag, size_t* result, size_t* len);

bool index2POSTag(size_t tag, char** result);

bool index2FeatureName(size_t tag, char** result);

bool index2FeatureValue(size_t tag, char** result);


bool buildDependencyTree(size_t* tags, size_t len, size_t* result);

bool getCompoundDeprelTag(size_t tag, size_t* result, size_t* len);

bool index2dependencyRelation(size_t tag, char** result);

bool index2dependencyRelationModifier(size_t tag, char** result);

#ifdef __cplusplus
}
#endif

#endif /* SUPPORT_C_INTERFACE */
