#pragma once

#include <boost/filesystem.hpp>

#include <vector>

#include <pkcs/Pkcs12.h>

namespace bf = boost::filesystem;

class OutputDirectory
{
  public:
    explicit OutputDirectory(const bf::path& dirPath);

    void savePkcs12(const Pkcs12& pkcs12);

  private:
    void saveData(bf::path path, const std::vector<uint8_t>& data);

  private:
    bf::path m_dirpath;
};
