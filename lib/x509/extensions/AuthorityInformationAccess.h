#pragma once

#include "X509Extension.h"

#include <string>
#include <vector>

class AuthorityInformationAccess : public X509Extension
{
public:
    static const int NID = NID_info_access;

    explicit AuthorityInformationAccess(const X509Extension& ext);
    explicit AuthorityInformationAccess(X509Extension&& ext);
    AuthorityInformationAccess(const AuthorityInformationAccess&) = default;
    AuthorityInformationAccess(AuthorityInformationAccess&&) = default;

    AuthorityInformationAccess& operator =(AuthorityInformationAccess&&) = default;
    AuthorityInformationAccess& operator =(const AuthorityInformationAccess&) = default;

    const std::string& oscp() const;
    const std::string& ca_issuer() const;

private:
    void parse();

private:
    std::string m_oscp;
    std::string m_caIssuer;
};


