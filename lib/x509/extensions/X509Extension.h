#pragma once

#include "utils/ObjectHolder.h"

#include <openssl/x509.h>

class X509Extension : public ObjectHolder<X509_EXTENSION, X509Extension>
{
  public:
    X509Extension(X509_EXTENSION* ext, bool acquire) noexcept
      : ObjectHolder(ext, acquire)
    {
    }

  public:
    friend class ObjectHelper;

    typedef X509_EXTENSION RawType;

    X509Extension(const X509Extension& other) = default;
    X509Extension(X509Extension&& other) = default;

    X509Extension& operator=(const X509Extension& other)
    {
        ObjectHolder::operator=(other);
        return *this;
    }

    X509Extension& operator=(X509Extension&& other) noexcept
    {
        ObjectHolder::operator=(std::move(other));
        return *this;
    }

    ~X509Extension() = default;

    static X509_EXTENSION* duplicate(X509_EXTENSION* pExt);
    static void destroy(RawType* raw) noexcept;

    bool is_critical() const noexcept;

    int nid() const noexcept;
};

