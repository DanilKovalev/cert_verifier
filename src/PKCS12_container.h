#pragma once

#include <openssl/pkcs12.h>
#include <vector>
#include <cstdint>

class PKCS12_container
{
public:

    static PKCS12_container from_der(const std::vector<uint8_t>& der);

private:
    PKCS12_container(PKCS12* pRaw);

private:
    PKCS12* m_pRaw;
};


