#include "X509Store.h"
#include "../SslException.h"
#include <utility>

X509Store::X509Store()
: m_store (X509_STORE_new())
, m_acquired(true)
{
    if(!m_store)
        throw SslException("X509_STORE_new");
}

X509Store::X509Store(X509_STORE *store, bool acquire) noexcept
 : m_store(store)
 , m_acquired(acquire)
{}

X509Store::X509Store(X509Store &&other) noexcept
 : m_store(std::exchange(other.m_store, nullptr))
 , m_acquired(std::exchange(other.m_acquired, false))
{}

X509Store::~X509Store()
{
    free();
}

void X509Store::free() noexcept
{
    if(m_store && m_acquired)
        X509_STORE_free(m_store);

    m_store = nullptr;
    m_acquired = false;
}

X509Store& X509Store::operator=(X509Store &&other) noexcept
{
    if (this == &other)
        return *this;

    std::swap(*this, other);
    other.free();

    return *this;
}

void X509Store::swap(X509Store& other) noexcept
{
    std::swap(this->m_store, other.m_store);
    std::swap(this->m_acquired, other.m_acquired);
}

X509_STORE *X509Store::raw()
{
    return m_store;
}

const X509_STORE *X509Store::raw() const
{
    return m_store;
}

void X509Store::loadFile(const std::string &path)
{
    if(X509_STORE_load_locations(m_store, path.c_str(), nullptr) != 1)
        throw SslException("X509_STORE_load_locations");
}

void X509Store::loadDirectory(const std::string &path)
{
    if(X509_STORE_load_locations(m_store, nullptr, path.c_str()) != 1)
        throw SslException("X509_STORE_load_locations");
}

void X509Store::loadDefaultLocation()
{
    if(X509_STORE_set_default_paths(m_store) != 1)
        throw SslException("X509_STORE_set_default_paths");
}

void X509Store::setTrust(bool flag)
{
    if(X509_STORE_set_trust(m_store, static_cast<int>(flag)) != 1)
        throw SslException("X509_STORE_set_trust");
}

