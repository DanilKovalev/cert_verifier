#pragma once

#include "Pkcs7.h"
#include "x509/X509Certificate.h"
#include "utils/StackOf.h"

class Pkcs7Signed : public Pkcs7
{
public:
    explicit Pkcs7Signed(Pkcs7&& );
    Pkcs7Signed(const Pkcs7Signed& other) = delete;
    Pkcs7Signed& operator=(const Pkcs7Signed& other) = delete;

    ~Pkcs7Signed();

    StackOf<X509Certificate> getCertificates() const;

private:
    PKCS7_SIGNED* m_pkcs7Signed;
};



