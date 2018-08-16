#pragma once

#include "x509/X509Certificate.h"

#include <string>
#include <vector>

std::string read_file(const std::string& path);
std::vector<uint8_t> read_binary_file(const std::string& path);
X509Certificate read_cert(const std::string& path);

