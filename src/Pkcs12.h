#pragma once

#include <openssl/pkcs12.h>
#include <vector>
#include <cstdint>

class Pkcs12
{
public:

    static Pkcs12 from_der(const uint8_t *bytes, size_t size);

private:
    Pkcs12(PKCS12* pkcs12, bool acquire);

private:
    PKCS12* m_pkcs12;
    bool m_acquired;
};


