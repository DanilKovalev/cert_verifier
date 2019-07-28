#pragma once

#include "utils/ObjectHelper.h"
#include "utils/ObjectHolder.h"

#include <openssl/x509.h>

class X509NameEntry : public ObjectHolder<X509_NAME_ENTRY, X509NameEntry>
{
    friend class ObjectHelper<X509NameEntry>;

    X509NameEntry(X509_NAME_ENTRY* raw, bool acquire) noexcept
      : ObjectHolder(raw, acquire)
    {
    }

  public:
    using RawType = X509_NAME_ENTRY;

    X509NameEntry(const X509NameEntry& other) = default;
    X509NameEntry(X509NameEntry&& other) = default;

    X509NameEntry& operator=(const X509NameEntry& other)
    {
        ObjectHolder::operator=(other);
        return *this;
    }

    X509NameEntry& operator=(const X509NameEntry&& other) noexcept
    {
        ObjectHolder::operator=(std::move(other));
        return *this;
    }

    ~X509NameEntry() = default;

    static RawType* duplicate(RawType* raw);
    static void destroy(RawType* raw) noexcept;

    std::string toString() const;
    int nid() const noexcept;
};
