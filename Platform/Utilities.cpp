#include "Utilities.h"
#include "log/LogSystem.h"
#include <fstream>

namespace Cola
{
    Utilities::Utilities(/* args */)
    {
    }

    Utilities::~Utilities()
    {
    }

    std::vector<char> Utilities::readFile(const std::string &filePath)
    {
        std::vector<char> buffer;
        std::ifstream file(filePath, std::ios::ate | std::ios::in | std::ios::binary); //从文件末尾开始读取

        if (!file.is_open())
        {
            LOG_ERROR("failed to open {0}!", filePath);
        }
        else
        {
            size_t fileSize = static_cast<size_t>(file.tellg());

            buffer.resize(fileSize);

            file.seekg(0, std::ios::beg);
            file.read(buffer.data(), fileSize);

            file.close();
        }

        return buffer;
    }
}