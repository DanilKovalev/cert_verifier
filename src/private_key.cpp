#include "private_key.h"
#include "bio/bio_ostring.h"
#include "bio/bio_istring.h"
#include "ssl_exc.h"

#include <openssl/pem.h>
#include <utility>

private_key::private_key(EVP_PKEY *pKey) noexcept
: m_pKey(pKey)
{

}

private_key::~private_key() noexcept
{
    free();
}

private_key::private_key(private_key&& rhs)
: m_pKey(std::exchange(rhs.m_pKey, nullptr))
{}

private_key& private_key::operator=(private_key&& rhs)
{
    m_pKey = std::exchange(rhs.m_pKey, nullptr);
    return *this;
}

EVP_PKEY* private_key::raw() noexcept
{
    return m_pKey;
}

const EVP_PKEY* private_key::raw() const noexcept
{
    return m_pKey;
}

private_key private_key::from_pem(const std::string& pem)
{
    bio_istring bio(&pem);

    EVP_PKEY* pRaw_key = nullptr;
    if( !PEM_read_bio_PrivateKey(bio.get_bio(), &pRaw_key, nullptr, nullptr) )
        throw ssl_exc("Failed to read private key");

    return private_key(pRaw_key);
}

std::string private_key::to_pem() const
{
    bio_ostring bio;

    if( !PEM_write_bio_PrivateKey(bio.get_bio(), m_pKey, nullptr, nullptr, 0, 0, nullptr))
        throw ssl_exc("Failed to write private key");

    return bio.detach_string();
}

void private_key::free()
{
    EVP_PKEY_free(m_pKey);
}

