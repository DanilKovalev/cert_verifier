#pragma once

#include "x509_extension_list.h"

#include <openssl/x509.h>

#include <string>
#include <vector>

class x509_certificate
{
public:
    explicit x509_certificate(X509* pCert, bool acquire);
    x509_certificate(const x509_certificate& rhs);
    x509_certificate(x509_certificate&& rhs) noexcept;

    ~x509_certificate() noexcept;

    x509_certificate& operator =(const x509_certificate& rhs);
    x509_certificate& operator =(x509_certificate&& rhs) noexcept;

    friend void swap(x509_certificate& a, x509_certificate& b) noexcept;

    std::vector<uint8_t> digest(const EVP_MD* type) const;
    std::string get_issuer_name() const;
    std::string get_subject_name() const;

    X509* raw();
    const X509* raw() const;

    static x509_certificate from_pem(const std::string& pem);
    std::string to_pem() const;

    x509_extension_list get_extensions();

private:
    static X509* duplicate(X509 *pCert);
    void free() noexcept;


private:
    X509* m_pCert;
    bool m_acquired;
};

void swap(x509_certificate& a, x509_certificate& b) noexcept;
