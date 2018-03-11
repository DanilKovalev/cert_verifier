#include "x509_certificate.h"

#include "bio/bio_istring.h"
#include "bio/bio_ostring.h"
#include "bio_guard.h"

#include <openssl/pem.h>

x509_certificate::x509_certificate(X509 *pCert)
: m_pCert(pCert)
{}

x509_certificate::x509_certificate(const x509_certificate& rhs)
: m_pCert(duplicate(rhs.m_pCert))
{}

x509_certificate::x509_certificate(x509_certificate&& rhs) noexcept
: m_pCert(rhs.m_pCert)
{
    rhs.m_pCert = nullptr;
}

x509_certificate& x509_certificate::operator =(x509_certificate&& rhs) noexcept
{
    this->m_pCert = std::exchange(rhs.m_pCert, nullptr);
    return *this;
}

x509_certificate::~x509_certificate()
{
    free();
}

x509_certificate& x509_certificate::operator =(const x509_certificate& rhs)
{
    if (this == &rhs)
        return *this;

    X509* pCert = duplicate(rhs.m_pCert);
    this->free();
    this->m_pCert = pCert;
    return *this;
}

std::vector<std::uint8_t> x509_certificate::digest(const EVP_MD* type) const
{
    std::vector<std::uint8_t> fingerprint(static_cast<size_t >(EVP_MD_size(type)), 0);
    if(X509_digest(m_pCert, type, fingerprint.data(), nullptr) != 1)
        throw std::runtime_error("X509_digest failed.");

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
        std::__throw_runtime_error("Failed to duplicate X509 certificate");

    return result;
}

void x509_certificate::free()
{
    if(m_pCert)
        X509_free(m_pCert);

    m_pCert = nullptr;
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
        std::__throw_runtime_error("Failed to read X509 certificate");

    return x509_certificate(pCert);
}


std::string x509_certificate::to_pem() const
{
    bio_ostring bio;
    if( !PEM_write_bio_X509(bio.get_bio(), m_pCert))
        std::__throw_runtime_error("Failed to write X509 certificate");

    return bio.detach_string();
}

/*std::vector<std::uint8_t> x509_certificate::to_der() const
{
    std::uint8_t* buffer = nullptr;
    ssize_t len = i2d_X509(m_pCert, &buffer);
    if (len <= 0)
        throw std::runtime_error("i2d_X509");

    std::vector<std::uint8_t> vector(buffer, buffer + len);
    ::free(buffer);
    return vector;
}

x509_certificate x509_certificate::from_der(const std::string &der)
{
    const uint8_t* rawData = reinterpret_cast<const uint8_t *>(der.c_str());
    X509* pCert;
    long len = static_cast<long>(der.length());
    if ( d2i_X509(&pCert, &rawData, len ) )
        throw std::runtime_error("d2i_X509 failed");

    return x509_certificate(pCert);
}*/

x509_certificate x509_certificate::create_wrapper(X509* pCert)
{
    return x509_certificate(duplicate(pCert));
}

x509_certificate x509_certificate::create_attached(X509* pCert)
{
    return x509_certificate(pCert);
}