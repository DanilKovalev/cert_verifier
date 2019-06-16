#pragma once

#include "extensions/X509Extension.h"
#include "extensions/X509ExtensionsStack.h"
#include "utils/ObjectHelper.h"
#include "utils/ObjectHolder.h"
#include "utils/StackOf.h"
#include "utils/X509Name.h"

#include <openssl/x509.h>

#include <string>
#include <utility>
#include <vector>

class X509Certificate : public ObjectHolder<X509, X509Certificate>
{
    friend class ObjectHelper<X509Certificate>;

  public: ///@todo: make private
    X509Certificate(X509* pCert, bool acquire) noexcept
      : ObjectHolder(pCert, acquire)
    {
    }

  public:
    using RawType = X509;

    X509Certificate(const X509Certificate& other) = default;
    X509Certificate(X509Certificate&& other) = default;

    X509Certificate& operator=(const X509Certificate& other)
    {
        ObjectHolder::operator=(other);
        return *this;
    }

    X509Certificate& operator=(X509Certificate&& other) noexcept
    {
        ObjectHolder::operator=(std::move(other));
        return *this;
    }

    ~X509Certificate() = default;

    std::vector<uint8_t> digest(const EVP_MD* type) const;
    X509Name getIssuerName() const;
    X509Name getSubjectName() const;

    bool isSelfSigned() const;

    static X509Certificate from_pem(const std::string& pem);
    static X509Certificate from_der(const std::vector<uint8_t>& pem);

    static X509* duplicate(X509* pCert);
    static void destroy(X509* pCert) noexcept;

    std::string to_pem() const;
    std::vector<uint8_t> to_der() const;
    X509ExtensionsStack get_extensions();

    bool hasExtensions() const noexcept;
};


