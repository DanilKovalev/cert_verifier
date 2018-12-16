#include "Pkcs12.h"

#include "bio/bio_guards.h"
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

    swap(rhs);
    rhs.free();
    return *this;
}

Pkcs12::~Pkcs12()
{
    free();
}

void Pkcs12::changePassword(const std::string& newPwd, const std::string& oldPwd)
{
    if (PKCS12_newpass(m_pkcs12, oldPwd.c_str(), newPwd.c_str()) != 1)
        throw SslException("PKCS12_newpass");
}

Pkcs12 Pkcs12::fromDer(const uint8_t *bytes, size_t size)
{
    PKCS12* pkcs12 = d2i_PKCS12(nullptr, &bytes, boost::numeric_cast<long>(size));
    if ( pkcs12 == nullptr)
        throw SslException("d2i_PKCS12");

    return Pkcs12(pkcs12, true);
}

std::vector<uint8_t> Pkcs12::toDer() const
{
    auto bio = createBioGuard(BIO_new(BIO_s_mem()));
    if(i2d_PKCS12_bio(bio.get(), m_pkcs12) != 1)
        throw SslException("i2d_X509");

    uint8_t* data = nullptr;
    long readSize = BIO_get_mem_data(bio.get(), &data);
    return std::vector<uint8_t>(data, data + readSize);
}

void Pkcs12::free() noexcept
{
    if (m_acquired)
        PKCS12_free(m_pkcs12);

    m_pkcs12 = nullptr;
    m_acquired = false;
}

void Pkcs12::swap(Pkcs12& other) noexcept
{
    std::swap(this->m_acquired, other.m_acquired);
    std::swap(this->m_pkcs12, other.m_pkcs12);
}

Pkcs12Data Pkcs12::parse(const std::string& pass)
{
    EVP_PKEY *pkey = nullptr;
    X509 *pCert = nullptr;
    STACK_OF(X509)* ca = nullptr;

    if(PKCS12_parse(m_pkcs12, pass.c_str(), &pkey, &pCert, &ca) != 1)
        throw SslException("PKCS12_parse");

    return Pkcs12Data{.pkey = PrivateKey(pkey, true),
                      .cert = X509Certificate(pCert, true),
                      .ca = StackOf<X509Certificate>(reinterpret_cast<struct stack_st*>(ca), true)};
}

Pkcs12 Pkcs12::create(Pkcs12Data& data, const std::string& pass)
{
    PKCS12* result = PKCS12_create(pass.c_str(), nullptr, data.pkey.raw(), data.cert.raw(), reinterpret_cast<stack_st_X509*>(data.ca.raw()), 0, 0, 0, 0, 0);
    if(result == nullptr)
        throw SslException("PKCS12_create");

    return Pkcs12(result, true);
}





