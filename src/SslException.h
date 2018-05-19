#pragma once

#include <string>
#include <exception>

class SslException : public std::exception
{
public:
    SslException();

    ~SslException() override ;
    explicit SslException(const std::string& errMsg );
    explicit SslException(ulong code);
    const char* what() const noexcept override;

    ulong getCode() const noexcept;

private:
    ulong m_code;
    std::string m_errorMsg;
};

