#pragma once

#include <string>
#include <exception>

class SslVerifyException : public std::exception
{
public:
    explicit SslVerifyException(long error) noexcept;
    SslVerifyException(const SslVerifyException& other) noexcept;
    SslVerifyException(SslVerifyException&& ex ) noexcept;

    ~SslVerifyException() override {};

    const char* what() const noexcept override;
    long getCode() const noexcept;

private:
    long m_error;
};

