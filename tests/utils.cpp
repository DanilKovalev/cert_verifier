#include "utils.h"
#include <fstream>

std::string read_file(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
        std::__throw_system_error(errno);

    std::string result ((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return result;
}
