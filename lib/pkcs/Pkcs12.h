#pragma once

#include "PrivateKey.h"
#include "utils/StackOf.h"
#include "x509/X509Certificate.h"

#include <openssl/pkcs12.h>

#include <string>
#include <vector>

struct Pkcs12Content
{
    PrivateKey pKey;
    X509Certificate cert;
    StackOf<X509Certificate> ca;

    static Pkcs12Content createEmpty();
};

class Pkcs12 : public ObjectHolder<PKCS12, Pkcs12>
{
  public:
    Pkcs12(PKCS12* raw, bool acquire)
      : ObjectHolder(raw, acquire)
    {
    }

    Pkcs12(const Pkcs12&) = delete;
    Pkcs12(Pkcs12&&) = default;
    Pkcs12& operator=(const Pkcs12&) = delete;
    Pkcs12& operator=(Pkcs12&& other) noexcept
    {
        ObjectHolder::operator=(std::move(other));
        return *this;
    }
    ~Pkcs12() = default;

    static void destroy(PKCS12* raw) noexcept;

    Pkcs12Content parse(const std::string& pass = "");
    static Pkcs12 create(Pkcs12Content& data, const std::string& pass = "");

    static Pkcs12 fromDer(const uint8_t* bytes, size_t size);
    std::vector<uint8_t> toDer() const;

    void changePassword(const std::string& newPwd, const std::string& oldPwd = "");
};
