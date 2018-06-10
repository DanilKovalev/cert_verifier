#pragma once

#include "x509/x509_certificate.h"

#include <openssl/pkcs12.h>

#include <string>
#include <vector>

class Pkcs12
{
public:
    Pkcs12(PKCS12* pkcs12, bool acquire);
    Pkcs12(const Pkcs12& rhs) = delete;
    Pkcs12(Pkcs12&& rhs) noexcept;
    Pkcs12& operator= (const Pkcs12& rhs) = delete;
    Pkcs12& operator= (Pkcs12&& rhs) noexcept;

    ~Pkcs12();

    friend void swap(Pkcs12& a, Pkcs12& b) noexcept;

    x509_certificate parse(const std::string& pass, std::vector<x509_certificate> ca);
    static Pkcs12 fromDer(const uint8_t *bytes, size_t size);

private:
    void free() noexcept;

private:
    PKCS12* m_pkcs12;
    bool m_acquired;
};

void swap(Pkcs12& a, Pkcs12& b) noexcept;
