#pragma once

#include "x509/X509Certificate.h"
#include "utils/StackOf.h"
#include "PrivateKey.h"

#include <openssl/pkcs12.h>

#include <string>
#include <vector>

struct Pkcs12Data
{
    PrivateKey pkey;
    X509Certificate cert;
    StackOf<X509Certificate> ca;
};

class Pkcs12
{
public:
    Pkcs12(PKCS12* pkcs12, bool acquire);
    Pkcs12(const Pkcs12& rhs) = delete;
    Pkcs12(Pkcs12&& rhs) noexcept;
    Pkcs12& operator= (const Pkcs12& rhs) = delete;
    Pkcs12& operator= (Pkcs12&& rhs) noexcept;
    ~Pkcs12();

    void swap(Pkcs12& other) noexcept;

    Pkcs12Data parse(const std::string& pass = "");
    static Pkcs12 create(Pkcs12Data& data, const std::string& pass = "");

    void changePassword(const std::string& newPwd, const std::string& oldPwd = "");
    static Pkcs12 fromDer(const uint8_t *bytes, size_t size);
    std::vector<uint8_t> toDer() const;

private:
    void free() noexcept;

private:
    PKCS12* m_pkcs12;
    bool m_acquired;
};


namespace std
{
    template <>
    inline void swap(Pkcs12& a, Pkcs12& b) noexcept
    {
        a.swap(b);
    }
}
