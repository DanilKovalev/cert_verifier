#include "X509Extension.h"
#include "../../SslException.h"
#include <openssl/x509v3.h>
#include <utility>
#include <memory>
#include <openssl/pkcs12.h>

X509Extension::X509Extension(X509_EXTENSION *ext, bool acquire)
: m_ext(ext)
, m_acquired(acquire)
{
}

X509Extension::X509Extension(const X509Extension &rhs)
 : m_ext(duplicate( rhs.m_ext) )
 , m_acquired(true)
{
}

X509Extension::X509Extension(X509Extension &&rhs) noexcept
 : m_ext(std::exchange(rhs.m_ext, nullptr))
 , m_acquired(rhs.m_acquired)
{
}

X509_EXTENSION *X509Extension::duplicate(X509_EXTENSION *pExt)
{
    X509_EXTENSION* result = X509_EXTENSION_dup(pExt);
    if(result == nullptr)
        throw SslException("Failed to duplicate X509 certificate");

    return result;
}

X509Extension::~X509Extension() noexcept
{
    free();
}

X509Extension& X509Extension::operator =(const X509Extension& rhs)
{
    if (this == &rhs)
        return *this;

    X509Extension tmp(rhs);
    swap(*this, tmp);
    return *this;
}


X509Extension& X509Extension::operator =(X509Extension&& rhs) noexcept
{
    this->m_ext = std::exchange(rhs.m_ext, nullptr);
    m_acquired = rhs.m_acquired;
    return *this;
}

void X509Extension::free() noexcept
{
    if(m_ext && m_acquired)
        X509_EXTENSION_free(m_ext);

    m_ext = nullptr;
}

X509_EXTENSION *X509Extension::raw()
{
    return m_ext;
}

const X509_EXTENSION *X509Extension::raw() const
{
    return m_ext;
}

X509_EXTENSION* X509Extension::detach() noexcept
{
    m_acquired = false;
    return std::exchange(m_ext, nullptr);

}

bool X509Extension::is_critical() const noexcept
{
    return static_cast<bool>(X509_EXTENSION_get_critical(m_ext));
}

int X509Extension::nid() const noexcept
{
    ASN1_OBJECT *obj = X509_EXTENSION_get_object(m_ext);
    return OBJ_obj2nid(obj);
}

X509Extension X509Extension::create_wrapper(X509_EXTENSION *ext)
{
    return X509Extension(ext, false);
}

X509Extension X509Extension::create_attached(X509_EXTENSION *ext)
{
    return X509Extension(ext, true);
}

void swap(X509Extension& a, X509Extension& b) noexcept
{
    std::swap(a.m_ext, b.m_ext);
    std::swap(a.m_acquired, b.m_acquired);
}

