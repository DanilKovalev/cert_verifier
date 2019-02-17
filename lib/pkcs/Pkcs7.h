#pragma once

#include <utility>
#include <vector>
#include <string>

#include <openssl/pkcs7.h>


class Pkcs7
{
public:
    Pkcs7(PKCS7* pkcs7, bool acquire) noexcept;

    Pkcs7(const Pkcs7& other);
    Pkcs7(Pkcs7&& other) noexcept;
    Pkcs7& operator= (const Pkcs7& other);
    Pkcs7& operator= (Pkcs7&& other) noexcept;
    ~Pkcs7();

    void swap(Pkcs7& other) noexcept;

    PKCS7* raw();
    const PKCS7* raw() const;
    PKCS7* detach();

    std::string toPem() const;
    static Pkcs7 fromPem(const std::string& pem);

    static Pkcs7 fromDer(const std::vector<uint8_t>& der );
    std::vector<uint8_t> toDer() const;

    static PKCS7* duplicate(PKCS7 *pkcs7);

    int nid() const;

private:
    void free() noexcept;

protected:
    PKCS7* m_pkcs7;
    bool m_acquired;

};

namespace std
{
    template <>
    inline void swap(Pkcs7& a, Pkcs7& b) noexcept
    {
        a.swap(b);
    }
}

