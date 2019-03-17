#include "X509Certificate.h"

#include "bio/bio_istring.h"
#include "bio/bio_ostring.h"
#include "bio/bio_guards.h"
#include "utils/X509Name.h"
#include "SslException.h"

#include <openssl/pem.h>

#include <boost/numeric/conversion/cast.hpp>

X509Certificate::X509Certificate(X509 *pCert, bool acquire) noexcept
: m_cert(pCert)
, m_acquired(acquire)
{}

X509Certificate::X509Certificate(const X509Certificate& rhs)
: m_cert(duplicate(rhs.m_cert))
, m_acquired(true)
{}

X509Certificate::X509Certificate(X509Certificate&& rhs) noexcept
: m_cert(std::exchange(rhs.m_cert, nullptr))
, m_acquired(std::exchange(rhs.m_acquired, false))
{}

X509Certificate& X509Certificate::operator =(X509Certificate&& rhs) noexcept
{
    if(this == &rhs)
        return *this;

    this->swap(rhs);
    rhs.free();
    return *this;
}

X509Certificate& X509Certificate::operator =(const X509Certificate& rhs)
{
    if (this == &rhs)
        return *this;

    X509Certificate temp(rhs);
    *this = std::move(temp);
    return *this;
}

X509Certificate::~X509Certificate() noexcept
{
    free();
}

std::vector<uint8_t> X509Certificate::digest(const EVP_MD* type) const
{
    std::vector<std::uint8_t> fingerprint(static_cast<size_t >(EVP_MD_size(type)), 0);
    if(X509_digest(m_cert, type, fingerprint.data(), nullptr) != 1)
        throw SslException("X509_digest failed.");

    return fingerprint;
}

std::string X509Certificate::get_issuer_name() const
{
    X509_NAME* name = X509_get_issuer_name(m_cert);
    if(name == nullptr)
        throw SslException("X509_get_issuer_name");

    return std::to_string(name);
}

std::string X509Certificate::get_subject_name() const
{
    X509_NAME* name = X509_get_subject_name(m_cert);
    if(name == nullptr)
        throw SslException("X509_get_subject_name");

    return std::to_string(name);
}

void X509Certificate::acquire()
{
    if (!m_acquired)
        m_cert = duplicate(m_cert);
}

X509* X509Certificate::duplicate(X509 *pCert)
{
    X509* result = X509_dup(pCert);
    if(result == nullptr)
        throw SslException("Failed to duplicate X509 certificate");

    return result;
}

void X509Certificate::free() noexcept
{
    if(m_cert && m_acquired)
        X509_free(m_cert);

    m_cert = nullptr;
    m_acquired = false;
}

X509 *X509Certificate::raw()
{
    return m_cert;
}

const X509* X509Certificate::raw() const
{
    return  m_cert;
}

X509* X509Certificate::detach() noexcept
{
    m_acquired = false;
    return std::exchange(m_cert, nullptr);
}

X509Certificate X509Certificate::from_pem(const std::string &pem)
{
    bio_istring bio(&pem);

    X509* pCert = nullptr;
    if( !PEM_read_bio_X509(bio.raw(), &pCert, nullptr, nullptr) )
        throw SslException("Failed to read X509 certificate");

    return X509Certificate(pCert, true);
}

X509Certificate X509Certificate::from_der(const std::vector<uint8_t>& der)
{
    X509* pCert = nullptr;
    const uint8_t *pData = der.data();
    if (!d2i_X509(&pCert, &pData, boost::numeric_cast<long>(der.size())))
        throw SslException("Failed to read X509 certificate from der format");

    return X509Certificate(pCert, true);
}

std::string X509Certificate::to_pem() const
{
    bio_ostring bio;
    if( !PEM_write_bio_X509(bio.get_bio(), m_cert))
        throw SslException("Failed to write X509 certificate");

    return bio.detach_string();
}

std::vector<uint8_t> X509Certificate::to_der() const
{
    auto bio = createBioGuard(BIO_new(BIO_s_mem()));
    if(i2d_X509_bio(bio.get(), m_cert) != 1)
        throw SslException("i2d_X509");

    uint8_t* data = nullptr;
    long readSize = BIO_get_mem_data(bio.get(), &data);
    return std::vector<uint8_t>(data, data + readSize);
}

StackOf<X509Extension> X509Certificate::get_extensions()
{
    return StackOf<X509Extension>(reinterpret_cast<const struct stack_st*>(X509_get0_extensions(m_cert)));
}

bool X509Certificate::hasExtensions() const noexcept
{
    return m_cert && X509_get_ext_count(m_cert);
}

void X509Certificate::swap(X509Certificate& other) noexcept
{
    std::swap(m_cert, other.m_cert);
    std::swap(m_acquired, other.m_acquired);
}

