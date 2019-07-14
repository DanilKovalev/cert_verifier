#include "X509Crl.h"

#include "SslException.h"
#include "bio/bio_istring.h"
#include "bio/bio_ostring.h"
#include "x509/extensions/X509Extension.h"

#include <boost/numeric/conversion/cast.hpp>

#include <openssl/pem.h>

X509Crl::RawType* X509Crl::duplicate(RawType* raw)
{
    X509_CRL* result = ::X509_CRL_dup(raw);
    if (result == nullptr)
        throw SslException("Failed to duplicate X509 CRL");

    return result;
}

void X509Crl::destroy(X509Crl::RawType* raw) noexcept
{
    ::X509_CRL_free(raw);
}

X509Crl X509Crl::fromPem(const std::string& pem)
{
    bio_istring bio(&pem);

    X509_CRL* raw = nullptr;
    if (!PEM_read_bio_X509_CRL(bio.raw(), &raw, nullptr, nullptr))
        throw SslException("Failed to read X509 CRL from pem format");

    return X509Crl(raw, true);
}

X509Crl X509Crl::fromDer(const std::vector<uint8_t>& der)
{
    X509_CRL* raw = nullptr;
    const uint8_t* pData = der.data();
    if (!d2i_X509_CRL(&raw, &pData, boost::numeric_cast<long>(der.size())))
        throw SslException("Failed to read X509 CRL from der format");

    return X509Crl(raw, true);
}

std::string X509Crl::toPem() const
{
    bio_ostring bio;
    if (!PEM_write_bio_X509_CRL(bio.get_bio(), m_raw))
        throw SslException("Failed to write X509 CRL to pem format");

    return bio.detach_string();
}

std::vector<uint8_t> X509Crl::toDer() const
{
    auto bio = createBioGuard(BIO_new(BIO_s_mem()));
    if (i2d_X509_CRL_bio(bio.get(), m_raw) != 1)
        throw SslException("i2d_X509");

    uint8_t* data = nullptr;
    long readSize = BIO_get_mem_data(bio.get(), &data);
    return std::vector<uint8_t>(data, data + readSize);
}

bool X509Crl::hasExtensions() const noexcept
{
    return X509_CRL_get_ext_count(m_raw);
}

StackOf<X509Extension> X509Crl::getExtensions()
{
    const X509_EXTENSIONS* extensions = X509_CRL_get0_extensions(m_raw);
    return StackOf<X509Extension>(reinterpret_cast<const struct stack_st*>(extensions));
}

X509Name X509Crl::getIssuer() const
{
    X509_NAME* name = X509_CRL_get_issuer(m_raw);
    if (name == nullptr)
        throw SslException("Failed to call get issuer for crl");

    return ObjectHelper<X509Name>::makeCopied(name);
}
