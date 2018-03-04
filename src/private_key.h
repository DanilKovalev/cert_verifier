#pragma once

#include <openssl/evp.h>

#include <string>

class private_key
{
public:
    explicit private_key(EVP_PKEY* pKey) noexcept;
    private_key(const private_key& rhs) = delete;
    private_key(private_key&& rhs);

    private_key& operator=(const private_key& rhs) = delete;
    private_key& operator=(private_key&& rhs);

    ~private_key() noexcept ;

    EVP_PKEY* raw() noexcept;
    const EVP_PKEY* raw() const noexcept;

    static private_key from_pem(const std::string & pem);
    std::string to_pem() const;

private:
    void free();

private:
    EVP_PKEY* m_pKey;
};

