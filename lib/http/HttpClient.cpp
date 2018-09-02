#include "HttpClient.h"
#include "HttpException.h"

#include <utility>
#include <stdexcept>

static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    (void)size;
    auto vector = static_cast<std::vector<uint8_t>*>(userdata);
    vector->insert(vector->end(), ptr, ptr + nmemb);
    return nmemb;
}

HttpClient::HttpClient(const std::string& url)
 : m_handler(createCurlHandler())
 , m_url(url)
{
}

HttpClient::HttpClient(HttpClient&& obj) noexcept
 : m_handler(std::exchange(obj.m_handler, nullptr))
 , m_url(std::move(obj.m_url))
{
}

HttpClient::~HttpClient()
{
    cleanup();
}

HttpClient& HttpClient::operator = (HttpClient&& obj) noexcept
{
    if (this == &obj)
        return *this;

    std::swap(*this, obj);
    obj.cleanup();

    return *this;
}

CURL* HttpClient::createCurlHandler()
{
    CURL* curl = curl_easy_init();
    if (!curl)
        throw std::runtime_error("Failed to initialize curl handler");
    return curl;
}

void HttpClient::cleanup() noexcept
{
    if(m_handler)
        curl_easy_cleanup(m_handler);

    m_handler = nullptr;
}

std::vector<uint8_t> HttpClient::perform()
{
    std::vector<uint8_t> body;

    curl_easy_setopt(m_handler, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(m_handler, CURLOPT_WRITEDATA, &body);
    curl_easy_setopt(m_handler, CURLOPT_URL, m_url.c_str());
    CURLcode curlCode = curl_easy_perform(m_handler);
    if (curlCode != CURLE_OK)
        throw HttpException(curlCode);

    long httpCode;
    curl_easy_getinfo(m_handler, CURLINFO_RESPONSE_CODE, &httpCode);

    if(httpCode != 200)
        throw HttpException(httpCode);

    return body;
}

void HttpClient::followLocation(bool flag)
{
    curl_easy_setopt(m_handler, CURLOPT_FOLLOWLOCATION, flag);
}

void HttpClient::swap(HttpClient& obj) noexcept
{
    std::swap(this->m_handler, obj.m_handler);
    std::swap(this->m_url, obj.m_url);
}

std::vector<uint8_t> HttpClient::request(const std::string& url, bool followLocation)
{
    HttpClient client(url);
    client.followLocation(followLocation);
    return client.perform();
}
