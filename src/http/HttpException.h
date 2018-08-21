#pragma once

#include <curl/curl.h>

#include <exception>
#include <string>

class HttpException : public std::exception
{
public:
    explicit HttpException(CURLcode curlCode);
    explicit HttpException(long httpCode);

    HttpException(const HttpException& other) = default;
    HttpException(HttpException&& ex ) = default;

    HttpException& operator=(const HttpException& other) = default;
    HttpException& operator=(HttpException& other) = default;

    virtual ~HttpException(){};
    virtual const char* what() const noexcept override;

private:
    std::string m_reason;
};
