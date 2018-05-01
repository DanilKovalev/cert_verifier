#include "general_name.h"

#include "../ssl_exc.h"

#include <openssl/x509v3.h>

#include <boost/numeric/conversion/cast.hpp>

#include <stdexcept>
#include <memory>


general_name::general_name(GENERAL_NAME *name, bool acquire) noexcept
: m_name(name)
, m_acquired(acquire)
{

}

std::string general_name::get_string()
{
    return to_string(m_name);
}

std::string to_string(GENERAL_NAME* name)
{
    if(name->type != GEN_URI)
        throw std::runtime_error("unsupported general name type ");

    unsigned char* out;
    int len = ASN1_STRING_to_UTF8(&out, name->d.uniformResourceIdentifier);
    if (len < 0)
        throw ssl_exc("ASN1_STRING_to_UTF8");
    std::unique_ptr<void , decltype(&CRYPTO_free)> ptr(static_cast<void*>(out), &CRYPTO_free);

    return std::string(reinterpret_cast<char *>(out), boost::numeric_cast<size_t >(len));
}


