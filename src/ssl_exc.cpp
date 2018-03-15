#include "ssl_exc.h"

#include <openssl/err.h>
#include <sstream>

static std::string sslErr2str(ulong code);

ssl_exc::ssl_exc(const std::string &text)
: m_error(text)
{
    m_error += "| " + sslErr2str(ERR_get_error());
}

const char* ssl_exc::what() const noexcept
{
    return  m_error.c_str();
}

std::string sslErr2str(ulong code)
{
    std::ostringstream stream;
    stream << "SSL error code: " << code << " ->" << ERR_error_string(code, nullptr);

    return stream.str();
}
