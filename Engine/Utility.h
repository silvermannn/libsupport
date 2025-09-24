#pragma once

#include <string>
#include <vector>

void toLower(std::string& s);

void ltrim(std::string &s);
void rtrim(std::string &s);
void trim(std::string &s);

bool parsePair(const std::string s, const std::string& delim, std::string& name, std::string& value);

std::vector<std::string> split(std::string s, std::string delims);

void filterWord(std::string& s);
