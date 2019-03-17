#pragma once

#include "Pkcs7.h"
#include "x509/X509Certificate.h"
#include "utils/StackOf.h"

class Pkcs7Signed : public Pkcs7
{
public:
    explicit Pkcs7Signed(Pkcs7&& );
    Pkcs7Signed(const Pkcs7Signed& other);
    Pkcs7Signed(Pkcs7Signed&& other) noexcept;
    Pkcs7Signed& operator=(const Pkcs7Signed& other);
    Pkcs7Signed& operator=(Pkcs7Signed&& other) noexcept;

    ~Pkcs7Signed();

    void swap(Pkcs7Signed& other) noexcept;

    StackOf<X509Certificate> getCertificates() const;

private:
    PKCS7_SIGNED* m_pkcs7Signed;
};

namespace std
{
    template <>
    inline void swap(Pkcs7Signed& a, Pkcs7Signed& b) noexcept
    {
        a.swap(b);
    }
}



