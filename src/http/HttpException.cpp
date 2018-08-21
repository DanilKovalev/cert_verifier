#include "HttpException.h"

HttpException::HttpException(CURLcode curlCode)
 : m_reason(curl_easy_strerror(curlCode))
{

}

HttpException::HttpException(long httpCode)
: m_reason("Bad http code: " + std::to_string(httpCode))
{
}

const char* HttpException::what() const noexcept
{
    return  m_reason.c_str();
}
