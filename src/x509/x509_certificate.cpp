#include "x509_certificate.h"

#include "../bio/bio_istring.h"
#include "../bio/bio_ostring.h"
#include "../bio/bio_guard.h"
#include "../SslException.h"

#include <openssl/pem.h>

x509_certificate::x509_certificate(X509 *pCert, bool acquire) noexcept
: m_pCert(pCert)
, m_acquired(acquire)
{}

x509_certificate::x509_certificate(const x509_certificate& rhs)
: m_pCert(duplicate(rhs.m_pCert))
, m_acquired(true)
{}

x509_certificate::x509_certificate(x509_certificate&& rhs) noexcept
: m_pCert(std::exchange(rhs.m_pCert, nullptr))
, m_acquired(std::exchange(rhs.m_acquired, false))
{}

x509_certificate& x509_certificate::operator =(x509_certificate&& rhs) noexcept
{
    if(this == &rhs)
        return *this;

    this->swap(rhs);
    rhs.free();
    return *this;
}

x509_certificate& x509_certificate::operator =(const x509_certificate& rhs)
{
    if (this == &rhs)
        return *this;

    x509_certificate temp(rhs);
    *this = std::move(temp);
    return *this;
}

x509_certificate::~x509_certificate() noexcept
{
    free();
}

std::vector<uint8_t> x509_certificate::digest(const EVP_MD* type) const
{
    std::vector<std::uint8_t> fingerprint(static_cast<size_t >(EVP_MD_size(type)), 0);
    if(X509_digest(m_pCert, type, fingerprint.data(), nullptr) != 1)
        throw SslException("X509_digest failed.");

    return fingerprint;
}

std::string x509_certificate::get_issuer_name() const
{
    char* name = X509_NAME_oneline(X509_get_issuer_name(m_pCert), nullptr, 0);

    std::string name_str(name);
    OPENSSL_free(name);

    return name_str;
}

std::string x509_certificate::get_subject_name() const
{
    char* name = X509_NAME_oneline(X509_get_subject_name(m_pCert), nullptr, 0);

    std::string name_str(name);
    OPENSSL_free(name);

    return name_str;
}


X509* x509_certificate::duplicate(X509 *pCert)
{
    X509* result = X509_dup(pCert);
    if(result == nullptr)
        throw SslException("Failed to duplicate X509 certificate");

    return result;
}

void x509_certificate::free() noexcept
{
    if(m_pCert && m_acquired)
        X509_free(m_pCert);

    m_pCert = nullptr;
    m_acquired = false;
}

X509 *x509_certificate::raw()
{
    return m_pCert;
}

const X509* x509_certificate::raw() const
{
    return  m_pCert;
}

x509_certificate x509_certificate::from_pem(const std::string &pem)
{
    bio_istring bio(&pem);

    X509* pCert = nullptr;
    if( !PEM_read_bio_X509(bio.get_bio(), &pCert, nullptr, nullptr) )
        throw SslException("Failed to read X509 certificate");

    return x509_certificate(pCert, true);
}


std::string x509_certificate::to_pem() const
{
    bio_ostring bio;
    if( !PEM_write_bio_X509(bio.get_bio(), m_pCert))
        throw SslException("Failed to write X509 certificate");

    return bio.detach_string();
}

x509_extension_list x509_certificate::get_extensions()
{
    return x509_extension_list(m_pCert->cert_info->extensions, false);
}

void x509_certificate::swap(x509_certificate& other) noexcept
{
    std::swap(m_pCert, other.m_pCert);
    std::swap(m_acquired, other.m_acquired);
}

