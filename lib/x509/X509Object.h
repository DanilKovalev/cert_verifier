#pragma once

#include "utils/ObjectHolder.h"
#include "X509Certificate.h"

#include <openssl/x509_vfy.h>

class X509Object : public ObjectHolder<X509_OBJECT, X509Object>
{
  public:
    using RawType = X509_OBJECT;

  public:
    X509Object();
    X509Object(X509_OBJECT* raw, bool acquire) noexcept
      : ObjectHolder(raw, acquire)
    {
    }

    X509Object(const X509Object& other) = delete;
    X509Object(X509Object&& other) = default;

    X509Object& operator=(const X509Object& other) = delete;
    X509Object& operator=(X509Object&& other) noexcept
    {
        ObjectHolder::operator=(std::move(other));
        return *this;
    }

    ~X509Object() = default;


    X509_LOOKUP_TYPE getType() const noexcept;
    X509Certificate toX509Certificate() const;

    static void destroy(X509_OBJECT* raw) noexcept;
};
