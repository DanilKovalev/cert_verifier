#include "X509VerifyParam.h"
#include "SslException.h"

#include <utility>
#include <openssl/x509v3.h>

struct X509_VERIFY_PARAM_ID_st
{
    STACK_OF(OPENSSL_STRING)* hosts; /* Set of acceptable names */
    unsigned int hostflags;     /* Flags to control matching features */
    char* peername;             /* Matching hostname in peer certificate */
    char* email;                /* If not NULL email address to match */
    size_t emaillen;
    unsigned char* ip;          /* If not NULL IP address to match */
    size_t iplen;               /* Length of IP address */
};


X509VerifyParam::X509VerifyParam()
 : m_raw(X509_VERIFY_PARAM_new())
 , m_acquired(true)
{
    if (m_raw == nullptr)
        throw SslException("X509_VERIFY_PARAM_new");
}

X509VerifyParam::X509VerifyParam(X509_VERIFY_PARAM* raw, bool acquire) noexcept
        : m_raw(raw), m_acquired(acquire)
{
}

X509VerifyParam::X509VerifyParam(X509VerifyParam&& other)
        : m_raw(std::exchange(other.m_raw, nullptr)), m_acquired(std::exchange(other.m_acquired, false))
{
}

X509VerifyParam& X509VerifyParam::operator=(X509VerifyParam&& other) noexcept
{
    if (this == &other)
        return *this;

    std::swap(other, *this);
    other.free();
    return *this;
}

X509VerifyParam::~X509VerifyParam()
{
    free();
}

void X509VerifyParam::free() noexcept
{
    if (m_acquired)
        X509_VERIFY_PARAM_free(m_raw);
    m_acquired = false;
    m_raw = nullptr;
}

void X509VerifyParam::swap(X509VerifyParam& other) noexcept
{
    std::swap(other.m_raw, this->m_raw);
    std::swap(other.m_acquired, this->m_acquired);
}

void X509VerifyParam::setHost(const std::string& host)
{
    if (X509_VERIFY_PARAM_set1_host(m_raw, host.c_str(), host.size()) != 1)
        throw SslException("X509_VERIFY_PARAM_set1_host");
}

void X509VerifyParam::setDepth(int depth) noexcept 
{
    X509_VERIFY_PARAM_set_depth(m_raw, depth);
}

X509_VERIFY_PARAM* X509VerifyParam::raw() noexcept
{
    return m_raw;
}

const X509_VERIFY_PARAM* X509VerifyParam::raw() const noexcept
{
    return m_raw;
}

X509_VERIFY_PARAM* X509VerifyParam::detach()
{
    if (!m_acquired)
        throw SslException("X509VerifyParam not acquired resource");

    m_acquired = false;
    X509_VERIFY_PARAM* result = std::exchange(m_raw, nullptr);

    return result;
}
