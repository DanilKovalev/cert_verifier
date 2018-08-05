#pragma once

#include "X509Store.h"
#include "X509Certificate.h"
#include "X509VerifyParam.h"
#include "SslException.h"
#include "utils/StackOf.h"

#include <openssl/x509_vfy.h>

class X509StoreCtx
{
public:
    X509StoreCtx();
    X509StoreCtx(const X509StoreCtx& other) = delete;
    X509StoreCtx(X509StoreCtx&& other) noexcept;

    X509StoreCtx& operator=(const X509StoreCtx& other) = delete;
    X509StoreCtx& operator=(X509StoreCtx&& other) noexcept;
    ~X509StoreCtx();

    X509_STORE_CTX* raw();
    const X509_STORE_CTX* raw() const;

    void verify(X509Certificate& cert);
    void setStore(X509Store&& store);

    void swap(X509StoreCtx& other) noexcept;

    void setParametrs(X509VerifyParam&& param) noexcept;
    void setAdditionalCertificates(const StackOf<X509Certificate>& certsChain);
    void setAdditionalCertificates(StackOf<X509Certificate>&& certsChain) noexcept;

    StackOf<X509Certificate> getChain();
    int getErrorDepth() const noexcept;

private:
    void free() noexcept;
    void setCertificate(X509Certificate &cert) noexcept;
    void init();
private:
    X509_STORE_CTX* m_raw;
    X509Store m_store;
    X509VerifyParam m_param;
    StackOf<X509Certificate> m_additionalCerts;
};

namespace std
{
    template <>
    inline void swap(X509StoreCtx& a, X509StoreCtx& b) noexcept
    {
        a.swap(b);
    }
}

