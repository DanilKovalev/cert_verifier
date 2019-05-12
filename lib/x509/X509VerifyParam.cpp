#include "X509VerifyParam.h"
#include "SslException.h"

#include <utility>
#include <openssl/x509v3.h>

X509VerifyParam::X509VerifyParam()
 : ObjectHolder(X509_VERIFY_PARAM_new(), true)
{
    if (m_raw == nullptr)
        throw SslException("X509_VERIFY_PARAM_new");
}

void X509VerifyParam::destroy(X509_VERIFY_PARAM* raw) noexcept
{
    X509_VERIFY_PARAM_free(raw);
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

