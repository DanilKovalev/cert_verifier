#pragma once

#include "openssl/asn1.h"

#include <string>

class Asn1String
{
  public:
    static std::string ToUtf8(const ASN1_STRING* asn1String);
};
