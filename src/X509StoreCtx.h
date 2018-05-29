#pragma once

#include "X509Store.h"
#include "x509_certificate.h"

#include <openssl/x509_vfy.h>

class X509StoreCtx
{
public:
    X509StoreCtx();
    X509StoreCtx(const X509StoreCtx& other) = delete;
    X509StoreCtx(X509StoreCtx&& ctx) noexcept;

    X509StoreCtx& operator=(const X509StoreCtx& other) = delete;
    X509StoreCtx& operator=(X509StoreCtx&& other) noexcept;
    ~X509StoreCtx();

    X509_STORE_CTX* raw();
    const X509_STORE_CTX* raw() const;

    void verify(x509_certificate& cert) noexcept;
    void setStore(X509Store&& store);

    void swap(X509StoreCtx& other) noexcept;

private:
    void free() noexcept;

private:
    X509_STORE_CTX* m_ctx;
    X509Store m_store;
};

namespace std
{
    template <>
    inline void swap(X509StoreCtx& a, X509StoreCtx& b) noexcept
    {
        a.swap(b);
    }
}

