#include "PKCS12_container.h"

#include <openssl/pkcs12.h>

#include <stdexcept>


PKCS12_container *PKCS12_container::from_der(const std::string& der)
{
    const uint8_t* rawData = reinterpret_cast<const uint8_t *>(der.c_str());
    PKCS12* pRaw;
    long len = static_cast<long>(der.length());
    if ( d2i_PKCS12(&pRaw, &rawData, len ) )
        throw std::runtime_error("d2i_PKCS12 failed");

    EVP_PKEY* raw_key;
    X509* raw_cert;
    stack_st_X509* raw_certs;

    if (PKCS12_parse(pRaw, nullptr, &raw_key, &raw_cert, &raw_certs) != 1)
        throw std::runtime_error("PKCS12_parse failed");


    PKCS12_container pkcs12_container;
}
