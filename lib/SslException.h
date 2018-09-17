#pragma once

#include <string>
#include <stdexcept>
#include <system_error>

const std::error_category& ssl_category() noexcept;

class SslException : public std::runtime_error
{
public:
    explicit SslException(const char* what);

    explicit SslException(int ec)
        : SslException(std::error_code(ec, ssl_category())) { }

    SslException(int ec, const char* what)
        : SslException(std::error_code(ec, ssl_category()), what) { }

    SslException(const SslException& other) = default;

    ~SslException() override = default;

    const std::error_code&
    code() const noexcept { return m_code; }

private:
    explicit SslException(const std::error_code& ec)
            : runtime_error(ec.message()), m_code(ec) { }

    SslException(const std::error_code& ec, const std::string& what)
            : runtime_error(what + ": " + ec.message()), m_code(ec) { }

private:
    std::error_code m_code;
};
