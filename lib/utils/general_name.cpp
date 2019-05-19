#include "general_name.h"

#include "../SslException.h"

#include <boost/numeric/conversion/cast.hpp>

#include <openssl/x509v3.h>

#include <memory>
#include <stdexcept>

general_name::general_name(GENERAL_NAME* name, bool acquire) noexcept
  : m_name(name)
  , m_acquired(acquire)
{
}

general_name::operator std::string() const
{
    return to_string(m_name);
}

std::string to_string(GENERAL_NAME* name)
{
    if (name->type != GEN_URI)
        throw std::runtime_error("unsupported general name type ");

    unsigned char* result;
    int len = ASN1_STRING_to_UTF8(&result, name->d.uniformResourceIdentifier);
    if (len < 0)
        throw SslException("ASN1_STRING_to_UTF8");

    auto deleter = [](unsigned char* out) { OPENSSL_free(out); };
    std::unique_ptr<unsigned char, decltype(deleter)> ptr(result, deleter);

    return std::string(reinterpret_cast<char*>(result), boost::numeric_cast<size_t>(len));
}

const GENERAL_NAME* general_name::raw() const
{
    return m_name;
}

GENERAL_NAME* general_name::raw()
{
    return  m_name;
}
