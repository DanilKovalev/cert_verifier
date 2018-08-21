#pragma once

#include <curl/curl.h>

#include <string>
#include <vector>

class HttpClient
{
public:
    explicit HttpClient(const std::string& url);
    HttpClient(const HttpClient& obj) = delete;
    HttpClient(HttpClient&& obj) noexcept;

    HttpClient& operator = (const HttpClient& obj) = delete;
    HttpClient& operator = (HttpClient&& obj) noexcept;

    ~HttpClient();

    void followLocation(bool flag);
    void swap(HttpClient& obj) noexcept;

    std::vector<uint8_t> perform();

    static std::vector<uint8_t> request(const std::string& url, bool followLocation = true);

private:
    static CURL* createCurlHandler();
    void cleanup() noexcept;

private:
    CURL* m_handler;
    std::string m_url;
};


namespace std
{
    template <>
    inline void swap(HttpClient& a, HttpClient& b) noexcept
    {
        a.swap(b);
    }
}