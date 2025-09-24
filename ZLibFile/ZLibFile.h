#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

#include <zlib.h>

class ZLibFile
{
    gzFile fileHande;
    bool fileIsOpen = false;

    const uint64_t CHUNK_SIZE = 1024 * 1024;

public:
    ZLibFile(const std::string& filename, bool write);
    ~ZLibFile();

    bool isOpen()
    {
        return fileIsOpen;
    }

    template<typename T>
    bool write(const T& t)
    {
        return gzwrite(fileHande, reinterpret_cast<const char*>(&t), sizeof(T)) == sizeof(T);
    }

    template<typename T>
    bool writePtr(const T* const t, uint64_t len)
    {
        const uint64_t chunks = (sizeof(T) * len) / CHUNK_SIZE;
        const uint64_t rest = (sizeof(T) * len) % CHUNK_SIZE;

        for (uint64_t i = 0; i < chunks; ++i)
        {
            gzwrite(fileHande, reinterpret_cast<const char*>(t) + i * CHUNK_SIZE, CHUNK_SIZE);
        }
        if (rest != 0)
        {
            gzwrite(fileHande, reinterpret_cast<const char*>(t) + chunks * CHUNK_SIZE, rest);
        }

        return true;
    }

    template<typename T>
    bool read(T& t)
    {
        return gzread(fileHande, reinterpret_cast<char*>(&t), sizeof(T)) == sizeof(T);
    }

    template<typename T>
    bool readPtr(T* t, uint64_t len)
    {
        const uint64_t chunks = (sizeof(T) * len) / CHUNK_SIZE;
        const uint64_t rest = (sizeof(T) * len) % CHUNK_SIZE;

        for (uint64_t i = 0; i < chunks; ++i)
        {
            if((uint64_t)gzread(fileHande, reinterpret_cast<char*>(t) + i * CHUNK_SIZE, CHUNK_SIZE) != CHUNK_SIZE)
            {
                return false;
            }
        }

        return (rest == 0) || ((uint64_t)gzread(fileHande, reinterpret_cast<char*>(t) + chunks * CHUNK_SIZE, rest) == rest);
    }

    template<typename K, typename V>
    bool write(const std::unordered_map<K, V>& m)
    {
        uint32_t size = m.size();
        if (!write(size))
        {
            return false;
        }

        for(const auto& [k, v]: m)
        {
            if (!write(k) || !write(v))
            {
                return false;
            }
        }

        return true;
    }

    template<typename K, typename V>
    bool read(std::unordered_map<K, V>& m)
    {
        m.clear();

        uint32_t size = 0;
        if (!read(size))
        {
            return false;
        }

        for (size_t i = 0; i < size; ++i)
        {
            K k;
            V v;
            if (!read(k) || !read(v))
            {
                return false;
            }

            m[k] = v;
        }

        return true;
    }

    template<typename E>
    bool write(const std::unordered_set<E>& m)
    {
        uint32_t size = m.size();
        if (!write(size))
        {
            return false;
        }

        for(const auto& e: m)
        {
            if (!write(e))
            {
                return false;
            }
        }

        return true;
    }

    template<typename E>
    bool read(std::unordered_set<E>& m)
    {
        m.clear();

        uint32_t size = 0;
        if (!read(size))
        {
            return false;
        }

        for (size_t i = 0; i < size; ++i)
        {
            E e;
            if (!read(e))
            {
                return false;
            }

            m.insert(e);
        }

        return true;
    }
};

template<>
bool ZLibFile::write<std::string>(const std::string& s);

template<>
bool ZLibFile::read<std::string>(std::string& s);
