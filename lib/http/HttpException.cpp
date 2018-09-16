#include "HttpException.h"

namespace
{
    struct curl_error_category : std::error_category
    {
        ~curl_error_category() override = default;

        const char* name() const noexcept override
        {
            return "curl";
        }

        std::string message(int ec) const override
        {
            return curl_easy_strerror(static_cast<CURLcode>(ec));
        }
    };

    struct http_error_category : std::error_category
    {
        ~http_error_category() override = default;

        const char* name() const noexcept override
        {
            return "http";
        }

        std::string message(int ec) const override
        {
            return std::to_string(ec);
        }
    };
}

const std::error_category& curl_category() noexcept
{
    static curl_error_category instance;
    return instance;
}

const std::error_category& http_category() noexcept
{
    static http_error_category instance;
    return instance;
}
