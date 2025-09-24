#include "ZLibFile.h"

#include <spdlog/spdlog.h>


ZLibFile::ZLibFile(const std::string& filename, bool write)
{
    fileHande = gzopen(filename.c_str(), write?"wb":"rb");

    fileIsOpen = fileHande != Z_NULL;

    if (!isOpen())
    {
        spdlog::error("Failed to open file {}: {}", filename, zError(errno));
    }
}

ZLibFile::~ZLibFile()
{
    gzclose(fileHande);
};

template<>
bool ZLibFile::write<std::string>(const std::string& s)
{
    uint32_t l = s.length();
    return write(l) && writePtr(s.c_str(), l);
}

template<>
bool ZLibFile::read<std::string>(std::string& s)
{
    uint32_t l = 0;
    if (!read(l))
        return false;

    s.resize(l);

    return readPtr((char*)s.data(), l);
}
