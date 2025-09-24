#include "Sentence.h"

void Sentence::saveBinary(ZLibFile& zfile) const
{
    uint32_t size = words.size();
    zfile.write(size);
    for (size_t i = 0; i < words.size(); ++i)
    {
        zfile.write(words[i]);
    }
}

bool Sentence::loadBinary(ZLibFile& zfile)
{
    uint32_t size = 0;
    if (!zfile.read(size))
    {
        return false;
    }

    words.resize(size);

    for (size_t i = 0; i < size; ++i)
    {
        if (!zfile.read( words[i]))
        {
            return false;
        }
    }
    
    return true;
}
