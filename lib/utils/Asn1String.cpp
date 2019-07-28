#include "Asn1String.h"

#include <boost/numeric/conversion/cast.hpp>

#include <memory>

#include <SslException.h>

std::string Asn1String::ToUtf8(const ASN1_STRING* asn1String)
{
    char* data;
    int len = ASN1_STRING_to_UTF8(reinterpret_cast<unsigned char**>(&data), asn1String);
    if (len < 0)
        throw SslException("ASN1_STRING_to_UTF8");

    auto deleter = [](char* out) { OPENSSL_free(out); };
    std::unique_ptr<char, decltype(deleter)> ptr(data, deleter);
    return std::string(data, boost::numeric_cast<size_t>(len));
}
