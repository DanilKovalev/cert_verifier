#include "Pkcs12.h"

#include "bio/bio_guard.h"
#include "SslException.h"

#include <boost/numeric/conversion/cast.hpp>

#include <stdexcept>

Pkcs12::Pkcs12(PKCS12* pkcs12, bool acquire)
: m_pkcs12(pkcs12)
, m_acquired(acquire)
{

}

Pkcs12::Pkcs12(Pkcs12 &&rhs) noexcept
: m_pkcs12(std::exchange(rhs.m_pkcs12, nullptr))
, m_acquired(std::exchange(rhs.m_acquired, false))
{
}

Pkcs12& Pkcs12::operator=(Pkcs12 &&rhs) noexcept
{
    if (this == &rhs)
        return *this;

    swap(*this, rhs);
    rhs.free();

    return *this;
}

Pkcs12::~Pkcs12()
{
    if (m_acquired)
        PKCS12_free(m_pkcs12);

    m_pkcs12 = nullptr;
}

Pkcs12 Pkcs12::fromDer(const uint8_t *bytes, size_t size)
{
    PKCS12* pkcs12 = d2i_PKCS12(nullptr, &bytes, boost::numeric_cast<long>(size));
    if ( pkcs12 == nullptr)
        throw SslException("d2i_PKCS12");

    return Pkcs12(pkcs12, true);
}

void Pkcs12::free() noexcept
{
    if (m_acquired)
        PKCS12_free(m_pkcs12);

    m_pkcs12 = nullptr;
    m_acquired = false;
}

void swap(Pkcs12& a, Pkcs12& b) noexcept
{
    std::swap(a.m_pkcs12, b.m_pkcs12);
    std::swap(a.m_acquired, b.m_acquired);
}

x509_certificate Pkcs12::parse(const std::string &pass, std::vector<x509_certificate> ca)
{
    (void) ca;

    EVP_PKEY *pkey;
    X509 *pCert;
    if(PKCS12_parse(m_pkcs12, pass.c_str(), &pkey, &pCert, nullptr) != 1)
        throw SslException("PKCS12_parse");

    EVP_PKEY_free(pkey);
    return x509_certificate(pCert, true);
}


