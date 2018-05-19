#include "SslException.h"

#include <openssl/err.h>
#include <sstream>

static std::string sslErr2str(ulong code);

SslException::SslException(const std::string &text)
 : SslException()
{
    m_errorMsg = text + " | Ssl error: " + m_errorMsg;
}

SslException::SslException()
 : m_code(ERR_get_error())
 , m_errorMsg(sslErr2str(m_code))
{}

SslException::SslException(ulong code)
 : m_code(code)
 , m_errorMsg(sslErr2str(m_code))
{}

const char* SslException::what() const noexcept
{
    return  m_errorMsg.c_str();
}

ulong SslException::getCode() const noexcept
{
    return m_code;
}

SslException::~SslException()
{}

std::string sslErr2str(ulong code)
{
    return ERR_error_string(code, nullptr);
}
