#include "OutputDirectory.h"

#include <boost/numeric/conversion/cast.hpp>

#include <iostream>

OutputDirectory::OutputDirectory(const bf::path& dirPath)
  : m_dirpath(dirPath)
  , m_crlCount(0)
{
    if (bf::exists(dirPath))
    {
        if (!bf::is_directory(dirPath))
            throw std::runtime_error("Not a directory");

        bf::remove_all(dirPath);
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

    if (bf::exists(fullPath))
        throw std::runtime_error("File already exist");

    if (!bf::exists(fullPath.parent_path()))
        bf::create_directories(fullPath.parent_path());

    std::ofstream fs(fullPath.string(), std::ios::out | std::ios::binary | std::ios::trunc);
    fs.write((const char*)data.data(), boost::numeric_cast<std::streamsize>(data.size()));
}

void OutputDirectory::saveCrl(const X509Crl& crl)
{
    std::string name = std::to_string(m_crlCount) + ".crl";
    bf::path fullPath = bf::path("crl")/name;

    saveData(fullPath, crl.toDer());
    m_crlCount++;
}
