#pragma once

#include "extensions/X509Extension.h"
#include "utils/StackOf.h"

#include <openssl/x509.h>

#include <string>
#include <vector>

class X509Certificate
{
public:
    typedef X509 RawType;

    X509Certificate(X509* pCert, bool acquire) noexcept;
    X509Certificate(const X509Certificate& rhs);
    X509Certificate(X509Certificate&& rhs) noexcept;

    ~X509Certificate() noexcept;

    X509Certificate& operator =(const X509Certificate& rhs);
    X509Certificate& operator =(X509Certificate&& rhs) noexcept;

    void swap(X509Certificate& other) noexcept;

    std::vector<uint8_t> digest(const EVP_MD* type) const;
    std::string getIssuerName() const;
    std::string getSubjectName() const;

    bool isSelfSigned() const;

    X509* raw();
    const X509* raw() const;
    X509* detach() noexcept;

    static X509Certificate from_pem(const std::string& pem);
    static X509Certificate from_der(const std::vector<uint8_t>& pem);

    void acquire();
    static X509* duplicate(X509 *pCert);

    std::string to_pem() const;
    std::vector<uint8_t> to_der() const;
    StackOf<X509Extension> get_extensions();

    bool hasExtensions() const noexcept;
private:
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

