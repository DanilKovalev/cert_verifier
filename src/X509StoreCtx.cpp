#include "X509StoreCtx.h"
#include "SslException.h"

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
    {
        return *this;
    }

    std::swap(*this, other);
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

void swap(X509StoreCtx& a, X509StoreCtx& b) noexcept
{
    std::swap(a.m_ctx, b.m_ctx);
}

void X509StoreCtx::setStore(X509Store&& store)
{
    m_store = std::move(store);
}

void X509StoreCtx::verify(x509_certificate& cert) noexcept
{
    X509_STORE_CTX_set_cert(m_ctx, cert.raw());
}
