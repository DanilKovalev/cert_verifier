#include "PrivateKey.h"

#include "SslException.h"
#include "bio/bio_istring.h"
#include "bio/bio_ostring.h"

#include <openssl/pem.h>

#include <utility>

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

    if (!PEM_write_bio_PrivateKey(bio.get_bio(), m_raw, nullptr, nullptr, 0, nullptr, nullptr))
        throw SslException("Failed to write private key");

    return bio.detach_string();
}

void PrivateKey::destroy(EVP_PKEY* raw) noexcept
{
    EVP_PKEY_free(raw);
}

