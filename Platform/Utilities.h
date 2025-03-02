#ifndef UTILITIES_H
#define UTILITIES_H
#include "ColaStart.h"

namespace Cola
{
    class Utilities
    {
    public:
        Utilities(/* args */);
        ~Utilities();
        static std::vector<char> readFile(const std::string &filePath);
    };
}
#endif