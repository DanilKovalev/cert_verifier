#pragma once
#include "utils/ObjectHolder.h"

#include <openssl/x509_vfy.h>

#include <string>

class X509VerifyParam : public ObjectHolder<X509_VERIFY_PARAM, X509VerifyParam>
{
  public:
    X509VerifyParam();
    X509VerifyParam(X509_VERIFY_PARAM* raw, bool acquire) noexcept
      : ObjectHolder(raw, acquire)
    {
    }
    X509VerifyParam(const X509VerifyParam& other) = delete;
    X509VerifyParam(X509VerifyParam&& other) = default;
    X509VerifyParam& operator=(X509VerifyParam&& other) noexcept
    {
        ObjectHolder::operator=(std::move(other));
        return *this;
    }

    X509VerifyParam& operator=(const X509VerifyParam& other) = delete;


    ~X509VerifyParam() = default;

    static void destroy(X509_VERIFY_PARAM* raw) noexcept;

    void setHost(const std::string& host);
    void setDepth(int depth) noexcept;

};

