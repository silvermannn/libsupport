#include "Utilites.h"

#include <cmath>
#include <cstdio>

std::string randomString()
{
    size_t len = 2 + std::rand() % 10000;

    std::string s;
    for (size_t i = 0; i < len; ++i)
    {
        s += 'a' + std::rand() % 26;
    }

    return s;
}