#pragma once

#include "extensions/X509Extension.h"
#include "utils/ObjectHelper.h"
#include "utils/ObjectHolder.h"
#include "utils/StackOf.h"
#include "x509/name/X509Name.h"

#include <openssl/x509.h>

#include <vector>

class X509Crl : ObjectHolder<X509_CRL, X509Crl>
{
    friend class ObjectHelper<X509Crl>;

  private:
    X509Crl(X509_CRL* raw, bool acquire) noexcept
      : ObjectHolder(raw, acquire)
    {
    }

  public:
    using RawType = X509_CRL;

    X509Crl(const X509Crl& other) = default;
    X509Crl(X509Crl&& other) = default;

    X509Crl& operator=(const X509Crl& other)
    {
        ObjectHolder::operator=(other);
        return *this;
    }

    X509Crl& operator=(X509Crl&& other) noexcept
    {
        ObjectHolder::operator=(std::move(other));
        return *this;
    }

    ~X509Crl() = default;

    static X509Crl fromPem(const std::string& pem);
    static X509Crl fromDer(const std::vector<uint8_t>& der);

    std::string toPem() const;
    std::vector<uint8_t> toDer() const;
    X509Name getIssuer() const;

    static RawType* duplicate(RawType* raw);
    static void destroy(RawType* raw) noexcept;

    StackOf<X509Extension> getExtensions();
    bool hasExtensions() const noexcept;
};
