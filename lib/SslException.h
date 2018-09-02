#pragma once

#include <string>
#include <exception>

class SslException : public std::exception
{
public:

    SslException() noexcept;
    SslException(const SslException& other) noexcept;
    explicit SslException(const std::string& errMsg ) noexcept;
    explicit SslException(ulong code) noexcept;
    SslException(SslException&& ex ) noexcept;

    ~SslException() override;

    const char* what() const noexcept override;
    ulong getCode() const noexcept;
    int getReasonCode() const noexcept;

private:
    ulong m_code;
    std::string m_errorMsg;
};

