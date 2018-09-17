#pragma once

#include <string>

#include <system_error>
#include <curl/curl.h>

const std::error_category& curl_category() noexcept;
const std::error_category& http_category() noexcept;

struct HttpCode
{
    explicit HttpCode(long code) : value(code) {};

    long value;
};

namespace std
{
    template<>
    struct is_error_code_enum<CURLcode> : public true_type { };

    inline error_code
    make_error_code(CURLcode __errc) noexcept
    { return error_code(static_cast<int>(__errc), curl_category()); }

    template<>
    struct is_error_code_enum<HttpCode> : public true_type { };

    inline error_code
    make_error_code(HttpCode __errc) noexcept
    { return error_code(static_cast<int>(__errc.value), http_category()); }
}


class HttpException : public std::runtime_error
{
public:
    HttpException() : std::runtime_error(curl_category().message(0)), m_code() {};

    explicit HttpException(CURLcode code)
            : HttpException(std::make_error_code(code)) {};

    explicit HttpException(HttpCode code)
            : HttpException(std::make_error_code(code)) {};

    HttpException(CURLcode code, const char* what)
            : HttpException(std::make_error_code(code), what) {}

    HttpException(HttpCode code, const char* what)
            : HttpException(std::make_error_code(code), what) {}

    HttpException(const HttpException& obj) = default;

    ~HttpException() override = default;

    const std::error_code& code() const noexcept { return m_code; }
private:
    explicit HttpException(const std::error_code& ec)
            : std::runtime_error(ec.message()), m_code(ec) {};

    HttpException(const std::error_code& ec, const std::string& what)
            : std::runtime_error(what + ": " + ec.message()), m_code(ec) {};

private:
    std::error_code m_code;
};

