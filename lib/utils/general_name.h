#pragma once

#include <openssl/x509v3.h>
#include <string>

class general_name
{
public:
    general_name( GENERAL_NAME* name, bool acquire ) noexcept;

    std::string get_string();

private:
    GENERAL_NAME* m_name;
    bool m_acquired;
};

std::string to_string(GENERAL_NAME* name);
