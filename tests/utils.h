#pragma once

#include <string>
#include <vector>

std::string read_file(const std::string& path);
std::vector<uint8_t> read_binary_file(const std::string& path);

