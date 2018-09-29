#pragma once

#include <string>

#include <openssl/x509.h>

class X509Name
{
public:
    X509Name(X509_NAME* raw, bool acquire) noexcept;

    std::string string() const;
private:
    X509_NAME* m_raw;
    bool m_acquired;
};

namespace std
{
    std::string to_string(const X509_NAME* raw);
}


