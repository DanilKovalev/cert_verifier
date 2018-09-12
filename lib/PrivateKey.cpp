#include "PrivateKey.h"
#include "bio/bio_ostring.h"
#include "bio/bio_istring.h"
#include "SslException.h"

#include <openssl/pem.h>
#include <utility>

PrivateKey::PrivateKey(EVP_PKEY *pKey) noexcept
: m_key(pKey)
{

}

PrivateKey::~PrivateKey() noexcept
{
    free();
}

PrivateKey::PrivateKey(PrivateKey&& rhs) noexcept
: m_key(std::exchange(rhs.m_key, nullptr))
{}

PrivateKey& PrivateKey::operator=(PrivateKey&& rhs) noexcept
{
    m_key = std::exchange(rhs.m_key, nullptr);
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

    return PrivateKey(pRaw_key);
}

std::string PrivateKey::to_pem() const
{
    bio_ostring bio;

    if( !PEM_write_bio_PrivateKey(bio.get_bio(), m_key, nullptr, nullptr, 0, nullptr, nullptr))
        throw SslException("Failed to write private key");

    return bio.detach_string();
}

void PrivateKey::free()
{
    EVP_PKEY_free(m_key);
}

