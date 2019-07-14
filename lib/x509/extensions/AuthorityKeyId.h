#pragma once

#include "X509Extension.h"

class AuthorityKeyId : public X509Extension
{
  public:
    static const int NID = NID_authority_key_identifier;

    explicit AuthorityKeyId(const X509Extension& ext);
    explicit AuthorityKeyId(X509Extension&& ext);


    AuthorityKeyId(const AuthorityKeyId&) = default;
    AuthorityKeyId(AuthorityKeyId&&) = default;

    ~AuthorityKeyId() = default;
};

