#include "Pkcs7.h"

#include <boost/numeric/conversion/cast.hpp>

#include "SslException.h"

#include "bio/bio_istring.h"
#include "bio/bio_ostring.h"
#include "bio/bio_guards.h"

#include <openssl/pem.h>

Pkcs7::Pkcs7(PKCS7* pkcs7, bool acquire) noexcept
: m_pkcs7(pkcs7)
, m_acquired(acquire)
{
}

Pkcs7::Pkcs7(const Pkcs7& rhs)
 : m_pkcs7(duplicate(rhs.m_pkcs7))
 , m_acquired(true)
{
}

Pkcs7::Pkcs7(Pkcs7&& other) noexcept
: m_pkcs7(std::exchange(other.m_pkcs7, nullptr))
, m_acquired(std::exchange(other.m_acquired, false))
{
}

Pkcs7& Pkcs7::operator= (const Pkcs7& other)
{
    if(this == &other)
        return *this;

    Pkcs7 tmp(other);
    this->swap(tmp);
    return *this;
}

Pkcs7& Pkcs7::operator= (Pkcs7&& other) noexcept
{
    if(this == &other)
        return *this;

    this->swap(other);
    other.free();
    return *this;
}

Pkcs7::~Pkcs7()
{
    free();
}

void Pkcs7::free() noexcept
{
    if (m_pkcs7 && m_acquired) {
        PKCS7_free(m_pkcs7);
    }

    m_pkcs7 = nullptr;
    m_acquired = false;
}

PKCS7* Pkcs7::duplicate(PKCS7 *pkcs7)
{
    PKCS7* result = PKCS7_dup(pkcs7);
    if (result == nullptr)
        throw  SslException("Failed to duplicate PKCS7");

    return result;
}

void Pkcs7::swap(Pkcs7& other) noexcept
{
    std::swap(this->m_pkcs7, other.m_pkcs7);
    std::swap(this->m_acquired, other.m_acquired);
}

PKCS7* Pkcs7::raw()
{
    return m_pkcs7;
}

const PKCS7* Pkcs7::raw() const
{
    return m_pkcs7;
}

PKCS7* Pkcs7::detach()
{
    if (!m_acquired)
        throw std::logic_error("Pkcs7 not acquired resource");

    m_acquired = false;
    return std::exchange(m_pkcs7, nullptr);
}

Pkcs7 Pkcs7::fromPem(const std::string& pem)
{
    bio_istring bio(&pem);

    PKCS7* pkcs7 = nullptr;
    if( !PEM_read_bio_PKCS7(bio.raw(), &pkcs7, nullptr, nullptr) )
        throw SslException("Failed to read pkcs7 from pem");

    return Pkcs7(pkcs7, true);
}

std::string Pkcs7::toPem() const
{
    bio_ostring bio;
    if( !PEM_write_bio_PKCS7(bio.get_bio(), m_pkcs7))
        throw SslException("Failed to write pkcs7 to pem");

    return bio.detach_string();
}

Pkcs7 Pkcs7::fromDer(const std::vector<uint8_t>& der)
{
    const uint8_t* data = der.data();
    PKCS7* pkcs7 = d2i_PKCS7(nullptr, &data, boost::numeric_cast<long>(der.size()));
    if ( pkcs7 == nullptr)
        throw SslException("d2i_PKCS12");

    return Pkcs7(pkcs7, true);
}

std::vector<uint8_t> Pkcs7::toDer() const
{
    auto bio = createBioGuard(BIO_new(BIO_s_mem()));
    if(i2d_PKCS7_bio(bio.get(), m_pkcs7) != 1)
        throw SslException("i2d_X509");

    uint8_t* data = nullptr;
    long readSize = BIO_get_mem_data(bio.get(), &data);
    return std::vector<uint8_t>(data, data + readSize);
}

int Pkcs7::nid() const
{
    return OBJ_obj2nid(m_pkcs7->type);
}

