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


std::vector<uint8_t> read_binary_file(const std::string& path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open())
        std::__throw_system_error(errno);

    std::vector<uint8_t > result ((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return result;
}

X509Certificate read_cert(const std::string& path)
{
    std::string pem = read_file(path);
    return X509Certificate::from_pem(pem);
}
