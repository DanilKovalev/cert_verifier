#pragma once

#include <openssl/x509.h>

#include <string>
#include <vector>


class x509_certificate
{
public:
    x509_certificate(const x509_certificate& rhs);
    x509_certificate(x509_certificate&& rhs) noexcept;
    ~x509_certificate() noexcept;

    x509_certificate& operator =(const x509_certificate& rhs);
    x509_certificate& operator =(x509_certificate&& rhs) noexcept;

    std::vector<std::uint8_t> digest(const EVP_MD* type) const;
    std::string get_issuer_name() const;
    std::string get_subject_name() const;

    X509* raw();
    const X509* raw() const ;

    static x509_certificate from_pem(const std::string& pem);
    std::string to_pem() const;

    /*static x509_certificate from_der(const std::string& der);
    std::vector<std::uint8_t> to_der() const;*/

    static x509_certificate create_wrapper(X509* pCert);
    static x509_certificate create_attached(X509* pCert);

private:
    static X509* duplicate(X509 *pCert);
    void free();

    explicit x509_certificate(X509* pCert);

private:
    X509* m_pCert;
};

