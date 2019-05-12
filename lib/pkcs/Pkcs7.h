#pragma once

#include "utils/ObjectHolder.h"

#include <openssl/pkcs7.h>

#include <string>
#include <utility>
#include <vector>

class Pkcs7 : public ObjectHolder<PKCS7, Pkcs7>
{
  public:
    Pkcs7(PKCS7* raw, bool acquire) noexcept
    : ObjectHolder(raw, acquire)
    {
    }

    Pkcs7(const Pkcs7& other) = default;
    Pkcs7(Pkcs7&& other) = default;

    Pkcs7& operator=(const Pkcs7& other)
    {
        ObjectHolder::operator=(other);
        return *this;
    }

    Pkcs7& operator=(Pkcs7&& other) noexcept
    {
        ObjectHolder::operator=(std::move(other));
        return *this;
    }

    ~Pkcs7() = default;

    std::string toPem() const;
    static Pkcs7 fromPem(const std::string& pem);

    static Pkcs7 fromDer(const std::vector<uint8_t>& der);
    std::vector<uint8_t> toDer() const;

    static PKCS7* duplicate(PKCS7* pkcs7);
    static void destroy(PKCS7* pCert) noexcept;

    int nid() const;
};

