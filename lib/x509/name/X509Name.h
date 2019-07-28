#pragma once

#include "X509NameEntry.h"
#include "utils/ObjectHolder.h"

#include <openssl/x509.h>

#include <optional>
#include <string>

class X509Name : public ObjectHolder<X509_NAME, X509Name>
{
  public:
    X509Name(X509_NAME* raw, bool acquire) noexcept
      : ObjectHolder(raw, acquire)
    {
    }

  public:
    using RawType = X509_NAME;

    X509Name(const X509Name& other) = default;
    X509Name(X509Name& other) = default;

    X509Name& operator=(const X509Name& other)
    {
        ObjectHolder::operator=(other);
        return *this;
    }

    X509Name& operator=(X509Name&& other) noexcept
    {
        ObjectHolder::operator=(std::move(other));
        return *this;
    }

    ~X509Name() = default;

    static X509_NAME* duplicate(X509_NAME* raw);
    static void destroy(X509_NAME* raw) noexcept;

    std::string toString() const;
    std::optional<X509NameEntry> findEntry(int nid);
    X509NameEntry getEntry(int nid);
};

namespace std
{
std::string to_string(const X509_NAME* raw);
}
