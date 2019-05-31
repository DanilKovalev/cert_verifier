#include "OutputDirectory.h"

#include <boost/numeric/conversion/cast.hpp>

OutputDirectory::OutputDirectory(const bf::path& dirPath)
  : m_dirpath(dirPath)
{
    if (bf::exists(dirPath))
    {
        if (bf::is_directory(dirPath))
            return;

        std::string error = "Path " + dirPath.string() + "already exist";
        throw std::runtime_error(error);
    }

    bf::create_directories(dirPath);
}

void OutputDirectory::savePkcs12(const Pkcs12& pkcs12)
{
    saveData("pkcs12.pfx", pkcs12.toDer());
}

void OutputDirectory::saveData(bf::path path, const std::vector<uint8_t>& data)
{
    bf::path fullPath = m_dirpath / path;

    std::ofstream fs(fullPath.string(), std::ios::out | std::ios::binary | std::ios::trunc);
    fs.write((const char*)data.data(), boost::numeric_cast<std::streamsize>(data.size()));
}
