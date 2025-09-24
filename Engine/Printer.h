#pragma once

#include <string>

class Printer
{
    size_t _size = 0;
    size_t _lastProgress = 0;
    size_t _progress = 0;
    std::string _header;
    std::string _message;
    bool _force = true;

    void show(void);
public:
    Printer(const std::string& header, size_t size);

    void incProgress(size_t inc = 1);

    void print(const std::string& message);
};
