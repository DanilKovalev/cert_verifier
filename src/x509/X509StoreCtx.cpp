#include "X509StoreCtx.h"

#include <utility>

X509StoreCtx::X509StoreCtx()
: m_raw(X509_STORE_CTX_new())
, m_store()
, m_param()
{
    if (!m_raw)
        throw SslException("X509_STORE_CTX_new");
}

X509StoreCtx::X509StoreCtx(X509StoreCtx&& other) noexcept
: m_raw(std::exchange(other.m_raw, nullptr))
, m_store(std::move(other.m_store))
, m_param(std::move(other.m_param))
{
}

X509StoreCtx& X509StoreCtx::operator=(X509StoreCtx&& other) noexcept
{
    if (this == &other)
        return *this;

    swap(other);
    return *this;
}

void X509StoreCtx::free() noexcept
{
    if (m_raw)
        X509_STORE_CTX_free(m_raw);

    m_raw = nullptr;
}

X509StoreCtx::~X509StoreCtx()
{
    free();
}

X509_STORE_CTX *X509StoreCtx::raw()
{
    return m_raw;
}

const X509_STORE_CTX *X509StoreCtx::raw() const
{
    return m_raw;
}

void X509StoreCtx::swap(X509StoreCtx& other) noexcept
{
    std::swap(m_raw, other.m_raw);
}

void X509StoreCtx::setStore(X509Store&& store)
{
    m_store = std::move(store);
}

void X509StoreCtx::verify(X509Certificate& cert)
{
    init();
    setCertificate(cert);
    X509_STORE_CTX_set0_param(m_raw, m_param.detach());
    if(X509_verify_cert(m_raw) != 1)
    {
        int err = X509_STORE_CTX_get_error(m_raw);
        throw std::runtime_error( X509_verify_cert_error_string(err) );
    }
}

void X509StoreCtx::setCertificate(X509Certificate &cert) noexcept
{
    X509_STORE_CTX_set_cert(m_raw, cert.raw() );
}

void X509StoreCtx::init()
{
    if (X509_STORE_CTX_init(m_raw, m_store.raw(), nullptr, nullptr) != 1)
        throw SslException("X509_STORE_CTX_init");
}

void X509StoreCtx::setParametrs(X509VerifyParam&& param) noexcept
{
    m_param = std::move(param);
}
