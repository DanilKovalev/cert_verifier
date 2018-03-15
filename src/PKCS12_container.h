#pragma once

#include "x509_certificate.h"

#include <openssl/evp.h>

#include <vector>

class PKCS12_container
{
public:
    PKCS12_container();

    const x509_certificate& get_certificate() const;
    x509_certificate& get_certificate();

    std::vector<x509_certificate>& get_additional_certificates() noexcept;
    const std::vector<x509_certificate>& get_additional_certificates() const noexcept;

    EVP_PKEY* get_private_key();
    const EVP_PKEY* get_private_key() const;

    bool is_have_certificate() const noexcept;
    bool is_have_private_key() const noexcept;

    static PKCS12_container* from_der(const std::string& der);
    std::string to_der() const;

private:
    x509_certificate* m_pCert;
    std::vector<x509_certificate> m_vecCerts;
    EVP_PKEY* m_private_key;
};


