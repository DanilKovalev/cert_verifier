#include "PrivateKey.h"
#include "bio/bio_ostring.h"
#include "bio/bio_istring.h"
#include "SslException.h"

#include <openssl/pem.h>
#include <utility>

PrivateKey::PrivateKey(EVP_PKEY *pKey, bool acquire) noexcept
: m_key(pKey)
, m_acquired(acquire)
{
}

PrivateKey::~PrivateKey() noexcept
{
    free();
}

PrivateKey::PrivateKey(PrivateKey&& rhs) noexcept
: m_key(std::exchange(rhs.m_key, nullptr))
, m_acquired(std::exchange(rhs.m_acquired, false))
{}

PrivateKey& PrivateKey::operator=(PrivateKey&& rhs) noexcept
{
    if (this == &rhs)
        return *this;

    swap(rhs);
    return *this;
}

EVP_PKEY* PrivateKey::raw() noexcept
{
    return m_key;
}

const EVP_PKEY* PrivateKey::raw() const noexcept
{
    return m_key;
}

PrivateKey PrivateKey::from_pem(const std::string& pem)
{
    bio_istring bio(&pem);

    EVP_PKEY* pRaw_key = nullptr;
    if( !PEM_read_bio_PrivateKey(bio.raw(), &pRaw_key, nullptr, nullptr) )
        throw SslException("Failed to read private key");

    return PrivateKey(pRaw_key, true);
}

std::string PrivateKey::to_pem() const
{
    bio_ostring bio;

    if( !PEM_write_bio_PrivateKey(bio.get_bio(), m_key, nullptr, nullptr, 0, nullptr, nullptr))
        throw SslException("Failed to write private key");

    return bio.detach_string();
}

void PrivateKey::free() noexcept
{
    if(m_acquired)
        EVP_PKEY_free(m_key);

    m_key = nullptr;
    m_acquired = false;
}

void PrivateKey::swap(PrivateKey& other) noexcept
{
    std::swap(this->m_acquired, other.m_acquired);
    std::swap(this->m_key, other.m_key);
}

