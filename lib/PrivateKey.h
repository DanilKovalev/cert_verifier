#pragma once

#include <openssl/evp.h>

#include <string>

class PrivateKey
{
public:
    PrivateKey(EVP_PKEY* pKey, bool acquire) noexcept;
    PrivateKey(const PrivateKey& rhs) = delete;
    PrivateKey(PrivateKey&& rhs) noexcept;
    PrivateKey& operator=(const PrivateKey& rhs) = delete;
    PrivateKey& operator=(PrivateKey&& rhs) noexcept;

    ~PrivateKey() noexcept;
    void swap(PrivateKey& other) noexcept;

    EVP_PKEY* raw() noexcept;
    const EVP_PKEY* raw() const noexcept;

    static PrivateKey from_pem(const std::string & pem);
    std::string to_pem() const;

private:
    void free() noexcept;

private:
    EVP_PKEY* m_key;
    bool m_acquired;
};

namespace std
{
    template <>
    inline void swap(PrivateKey& a, PrivateKey& b) noexcept
    {
        a.swap(b);
    }
}
