#pragma once

#include <string>
#include <stdexcept>
#include <system_error>

const std::error_category& ssl_verify_category() noexcept;

class SslVerifyException : public std::runtime_error
{
public:
    SslVerifyException() : std::runtime_error(ssl_verify_category().message(0)), m_code() {};

    explicit SslVerifyException(long ec)
        : SslVerifyException(std::error_code(static_cast<int>(ec), ssl_verify_category())) {};

    SslVerifyException(long ec, const char* what)
            : SslVerifyException(std::error_code(static_cast<int>(ec), ssl_verify_category()), what) { }

    SslVerifyException(const SslVerifyException& obj)
        : std::runtime_error(obj), m_code(obj.code()) {}

    virtual ~SslVerifyException() noexcept {};

    const std::error_code& code() const noexcept { return m_code; }

private:
    explicit SslVerifyException(const std::error_code& ec)
        : std::runtime_error(ec.message()), m_code(ec) {};

    SslVerifyException(const std::error_code& ec, const std::string& what)
        : std::runtime_error(what + ": " + ec.message()), m_code(ec) {};

private:
    std::error_code m_code;
};

