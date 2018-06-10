#include "X509StoreCtx.h"
#include "../SslException.h"

#include <utility>

X509StoreCtx::X509StoreCtx()
: m_ctx(X509_STORE_CTX_new())
, m_store()
{
    if (!m_ctx)
    {
        throw SslException("X509_STORE_CTX_new");
    }
}

X509StoreCtx::X509StoreCtx(X509StoreCtx&& ctx) noexcept
: m_ctx(std::exchange(ctx.m_ctx, nullptr))
, m_store()
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
    if (m_ctx)
    {
        X509_STORE_CTX_free(m_ctx);
    }

    m_ctx = nullptr;
}

X509StoreCtx::~X509StoreCtx()
{
    free();
}

X509_STORE_CTX *X509StoreCtx::raw()
{
    return m_ctx;
}

const X509_STORE_CTX *X509StoreCtx::raw() const
{
    return m_ctx;
}

void X509StoreCtx::swap(X509StoreCtx& other) noexcept
{
    std::swap(m_ctx, other.m_ctx);
}

void X509StoreCtx::setStore(X509Store&& store)
{
    m_store = std::move(store);
}

void X509StoreCtx::verify(x509_certificate& cert)
{
    if (X509_STORE_CTX_init(m_ctx, m_store.raw(), cert.raw(), nullptr) != 1)
        throw SslException("X509_STORE_CTX_init");

    if(X509_verify_cert(m_ctx) != 1)
    {
        int err = X509_STORE_CTX_get_error(m_ctx);
        throw std::runtime_error( X509_verify_cert_error_string(err) );
    }
}

void X509StoreCtx::setCert(x509_certificate& cert) noexcept
{
    X509_STORE_CTX_set_cert(m_ctx, cert.raw() );
}
