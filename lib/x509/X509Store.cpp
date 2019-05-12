#include "X509Store.h"

#include "../SslException.h"

#include <utility>

X509Store::X509Store()
  : ObjectHolder(X509_STORE_new(), true)
{
    if (!m_raw)
        throw SslException("X509_STORE_new");
}

void X509Store::loadFile(const std::string& path)
{
    if (X509_STORE_load_locations(m_raw, path.c_str(), nullptr) != 1)
        throw SslException("X509_STORE_load_locations");
}

void X509Store::loadDirectory(const std::string& path)
{
    if (X509_STORE_load_locations(m_raw, nullptr, path.c_str()) != 1)
        throw SslException("X509_STORE_load_locations");
}

void X509Store::loadDefaultLocation()
{
    if (X509_STORE_set_default_paths(m_raw) != 1)
        throw SslException("X509_STORE_set_default_paths");
}

void X509Store::setTrust(bool flag)
{
    if (X509_STORE_set_trust(m_raw, static_cast<int>(flag)) != 1)
        throw SslException("X509_STORE_set_trust");
}

void X509Store::destroy(X509_STORE* raw) noexcept
{
    X509_STORE_free(raw);
}