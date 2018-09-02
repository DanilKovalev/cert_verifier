#include "SslVerifyException.h"

#include <openssl/x509.h>

SslVerifyException::SslVerifyException() noexcept
: m_error(0)
{
}

SslVerifyException::SslVerifyException(long error) noexcept
 : m_error(error)
{
}

SslVerifyException::SslVerifyException(const SslVerifyException& other) noexcept
 : m_error(other.m_error)
{
}

SslVerifyException::SslVerifyException(SslVerifyException&& other) noexcept
 : m_error(other.m_error)
{
}

SslVerifyException& SslVerifyException::operator=(const SslVerifyException& other) noexcept
{
    m_error = other.getCode();
    return *this;
}

const char* SslVerifyException::what() const noexcept
{
    return X509_verify_cert_error_string(m_error);
}

long SslVerifyException::getCode() const noexcept
{
    return m_error;
}

