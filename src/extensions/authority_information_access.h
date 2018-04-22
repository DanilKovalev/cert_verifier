#pragma once

#include "../x509_extension.h"

#include <string>
#include <vector>

class authority_information_access : public x509_extension
{
public:
    explicit authority_information_access(x509_extension& ext);

    const std::string& oscp() const;
    const std::string& ca_issuer() const;

private:
    void parse();

private:
    std::string m_oscp;
    std::string m_ca_issuers;
};


