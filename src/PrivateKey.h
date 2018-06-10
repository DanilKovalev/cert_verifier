#pragma once

#include <openssl/evp.h>

#include <string>

class PrivateKey
{
public:
    explicit PrivateKey(EVP_PKEY* pKey) noexcept;
    PrivateKey(const PrivateKey& rhs) = delete;
    PrivateKey(PrivateKey&& rhs) noexcept;

    PrivateKey& operator=(const PrivateKey& rhs) = delete;
    PrivateKey& operator=(PrivateKey&& rhs) noexcept;

    ~PrivateKey() noexcept ;

    EVP_PKEY* raw() noexcept;
    const EVP_PKEY* raw() const noexcept;

    static PrivateKey from_pem(const std::string & pem);
    std::string to_pem() const;

private:
    void free();

private:
    EVP_PKEY* m_key;
};

