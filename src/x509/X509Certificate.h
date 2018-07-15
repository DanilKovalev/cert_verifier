#pragma once

#include "extensions/X509Extension.h"
#include "utils/StackOf.h"

#include <openssl/x509.h>

#include <string>
#include <vector>

class X509Certificate
{
public:
    X509Certificate(X509* pCert, bool acquire) noexcept;
    X509Certificate(const X509Certificate& rhs);
    X509Certificate(X509Certificate&& rhs) noexcept;

    ~X509Certificate() noexcept;

    X509Certificate& operator =(const X509Certificate& rhs);
    X509Certificate& operator =(X509Certificate&& rhs) noexcept;

    void swap(X509Certificate& other) noexcept;

    std::vector<uint8_t> digest(const EVP_MD* type) const;
    std::string get_issuer_name() const;
    std::string get_subject_name() const;

    X509* raw();
    const X509* raw() const;

    static X509Certificate from_pem(const std::string& pem);
    std::string to_pem() const;

    StackOf<X509Extension> get_extensions();

private:
    static X509* duplicate(X509 *pCert);
    void free() noexcept;

private:
    X509* m_cert;
    bool m_acquired;
};

namespace std
{
    template <>
    inline void swap(X509Certificate& a, X509Certificate& b) noexcept
    {
        a.swap(b);
    }

}

