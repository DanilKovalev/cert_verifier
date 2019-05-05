#include "X509Certificate.h"

#include "SslException.h"
#include "bio/bio_guards.h"
#include "bio/bio_istring.h"
#include "bio/bio_ostring.h"
#include "utils/X509Name.h"

#include <boost/numeric/conversion/cast.hpp>

#include <openssl/pem.h>

std::vector<uint8_t> X509Certificate::digest(const EVP_MD* type) const
{
    std::vector<std::uint8_t> fingerprint(static_cast<size_t>(EVP_MD_size(type)), 0);
    if (X509_digest(m_raw, type, fingerprint.data(), nullptr) != 1)
        throw SslException("X509_digest failed.");

    return fingerprint;
}

std::string X509Certificate::getIssuerName() const
{
    X509_NAME* name = X509_get_issuer_name(m_raw);
    if (name == nullptr)
        throw SslException("X509_get_issuer_name");

    return std::to_string(name);
}

std::string X509Certificate::getSubjectName() const
{
    X509_NAME* name = X509_get_subject_name(m_raw);
    if (name == nullptr)
        throw SslException("X509_get_subject_name");

    return std::to_string(name);
}

bool X509Certificate::isSelfSigned() const
{
    return getIssuerName() == getSubjectName();
}

X509* X509Certificate::duplicate(X509* pCert)
{
    X509* result = X509_dup(pCert);
    if (result == nullptr)
        throw SslException("Failed to duplicate X509 certificate");

    return result;
}

void X509Certificate::destroy(X509* pCert) noexcept
{
    X509_free(pCert);
}

X509Certificate X509Certificate::from_pem(const std::string& pem)
{
    bio_istring bio(&pem);

    X509* pCert = nullptr;
    if (!PEM_read_bio_X509(bio.raw(), &pCert, nullptr, nullptr))
        throw SslException("Failed to read X509 certificate");

    return X509Certificate(pCert, true);
}

X509Certificate X509Certificate::from_der(const std::vector<uint8_t>& der)
{
    X509* pCert = nullptr;
    const uint8_t* pData = der.data();
    if (!d2i_X509(&pCert, &pData, boost::numeric_cast<long>(der.size())))
        throw SslException("Failed to read X509 certificate from der format");

    return X509Certificate(pCert, true);
}

std::string X509Certificate::to_pem() const
{
    bio_ostring bio;
    if (!PEM_write_bio_X509(bio.get_bio(), m_raw))
        throw SslException("Failed to write X509 certificate");

    return bio.detach_string();
}

std::vector<uint8_t> X509Certificate::to_der() const
{
    auto bio = createBioGuard(BIO_new(BIO_s_mem()));
    if (i2d_X509_bio(bio.get(), m_raw) != 1)
        throw SslException("i2d_X509");

    uint8_t* data = nullptr;
    long readSize = BIO_get_mem_data(bio.get(), &data);
    return std::vector<uint8_t>(data, data + readSize);
}

StackOf<X509Extension> X509Certificate::get_extensions()
{
    X509_EXTENSIONS* extensions = sk_X509_EXTENSION_dup(X509_get0_extensions(m_raw));
    return StackOf<X509Extension>(reinterpret_cast<struct stack_st*>(extensions));
}

bool X509Certificate::hasExtensions() const noexcept
{
    return m_raw && X509_get_ext_count(m_raw);
}

