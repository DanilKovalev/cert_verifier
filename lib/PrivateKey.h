#pragma once

#include "utils/ObjectHolder.h"

#include <openssl/evp.h>

#include <string>

class PrivateKey : public ObjectHolder<EVP_PKEY, PrivateKey>
{
  public:
    PrivateKey(EVP_PKEY* raw, bool acquire) noexcept
      : ObjectHolder(raw, acquire)
    {
    }
    PrivateKey(const PrivateKey& other) = delete;
    PrivateKey(PrivateKey&& other) = default;
    PrivateKey& operator=(const PrivateKey& other) = delete;
    PrivateKey& operator=(PrivateKey&& other) noexcept
    {
        ObjectHolder::operator=(std::move(other));
        return *this;
    }

    ~PrivateKey() = default;

    static PrivateKey from_pem(const std::string& pem);
    std::string to_pem() const;

    static void destroy(EVP_PKEY* raw) noexcept;
};

