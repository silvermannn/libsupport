#include "Utility.h"

#include <sstream>

#include <unicode/unistr.h>
#include <unicode/ustream.h>
#include <unicode/locid.h>

void toLower(std::string& s)
{
    static const icu::UnicodeString je("е", "UTF-8");
    static const icu::UnicodeString jo("ё", "UTF-8");
    icu::UnicodeString us(s.c_str(), "UTF-8");
    us.toLower();
    us.findAndReplace(jo, je);
    s.clear();
    us.toUTF8String(s);
}

void ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

void rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

void trim(std::string &s)
{
    rtrim(s);
    ltrim(s);
}

bool parsePair(const std::string s, const std::string& delim, std::string& name, std::string& value)
{
    const std::string::size_type pos = s.find(delim);
    if (pos == std::string::npos)
    {
        return false;
    }

    name = s.substr(0, pos);
    trim(name);
    value = s.substr(pos + 1);
    trim(value);
    return true;
}

std::vector<std::string> split(std::string s, std::string delims)
{
    std::for_each(delims.begin(), delims.end(), [&](char &c) { std::replace(s.begin(), s.end(), c, '\n'); });

    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> result;
    while(std::getline(ss, item))
    {
        trim(item);
        if (!item.empty())
        {
            result.push_back(item);
        }
    }

    return result;
}

void filterWord(std::string& s)
{
    bool replaced = false;
    std::replace_if(s.begin(), s.end(), [&replaced](unsigned char c){ bool r = std::isdigit(c); replaced = r || replaced; return r; }, 'N');
    if (replaced)
    {
        auto last = std::unique(s.begin(), s.end());
        s.erase(last, s.end());
    }
}
