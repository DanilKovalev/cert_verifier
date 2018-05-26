#include "SslException.h"

#include <openssl/err.h>
#include <sstream>

static std::string sslErr2str(ulong code);

SslException::SslException(const std::string &text) noexcept
: SslException()
{
    m_errorMsg = text + " | Ssl error: " + m_errorMsg;
}

SslException::SslException() noexcept
 : m_code(ERR_get_error())
 , m_errorMsg(sslErr2str(m_code))
{}

SslException::SslException(const SslException& other) noexcept
: m_code(other.m_code)
, m_errorMsg(other.m_errorMsg)
{
}

SslException::SslException(ulong code) noexcept
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



SslException::SslException(SslException &&ex) noexcept
: m_code(ex.m_code)
, m_errorMsg(std::move(ex.m_errorMsg))
{

}

int SslException::getReasonCode() const noexcept
{
    return ERR_GET_REASON(m_code);
}

SslException::~SslException() = default;

std::string sslErr2str(ulong code)
{
    return ERR_error_string(code, nullptr);
}
