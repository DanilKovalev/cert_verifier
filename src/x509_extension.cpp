#include "x509_extension.h"
#include "SslException.h"
#include <openssl/x509v3.h>
#include <utility>
#include <memory>
#include <openssl/pkcs12.h>

x509_extension::x509_extension(X509_EXTENSION *ext, bool acquire)
: m_ext(ext)
, m_acquired(acquire)
{
}

x509_extension::x509_extension(const x509_extension &rhs)
 : m_ext(duplicate( rhs.m_ext) )
 , m_acquired(true)
{
}

x509_extension::x509_extension(x509_extension &&rhs) noexcept
 : m_ext(std::exchange(rhs.m_ext, nullptr))
 , m_acquired(rhs.m_acquired)
{
}

X509_EXTENSION *x509_extension::duplicate(X509_EXTENSION *pExt)
{
    X509_EXTENSION* result = X509_EXTENSION_dup(pExt);
    if(result == nullptr)
        throw SslException("Failed to duplicate X509 certificate");

    return result;
}

x509_extension::~x509_extension() noexcept
{
    free();
}

x509_extension& x509_extension::operator =(const x509_extension& rhs)
{
    if (this == &rhs)
        return *this;

    x509_extension tmp(rhs);
    swap(*this, tmp);
    return *this;
}


x509_extension& x509_extension::operator =(x509_extension&& rhs) noexcept
{
    this->m_ext = std::exchange(rhs.m_ext, nullptr);
    m_acquired = rhs.m_acquired;
    return *this;
}

void x509_extension::free() noexcept
{
    if(m_ext && m_acquired)
        X509_EXTENSION_free(m_ext);

    m_ext = nullptr;
}

X509_EXTENSION *x509_extension::raw()
{
    return m_ext;
}

const X509_EXTENSION *x509_extension::raw() const
{
    return m_ext;
}

bool x509_extension::is_critical() const noexcept
{
    return static_cast<bool>(X509_EXTENSION_get_critical(m_ext));
}

int x509_extension::nid() const noexcept
{
    ASN1_OBJECT *obj = X509_EXTENSION_get_object(m_ext);
    return OBJ_obj2nid(obj);
}

x509_extension x509_extension::create_wrapper(X509_EXTENSION *ext)
{
    return x509_extension(ext, false);
}

x509_extension x509_extension::create_attached(X509_EXTENSION *ext)
{
    return x509_extension(ext, true);
}

void swap(x509_extension& a, x509_extension& b) noexcept
{
    std::swap(a.m_ext, b.m_ext);
    std::swap(a.m_acquired, b.m_acquired);
}

