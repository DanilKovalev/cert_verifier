#pragma once

#include "utils/ObjectHolder.h"

#include <openssl/x509_vfy.h>

#include <string>

class X509Store : public ObjectHolder<X509_STORE, X509Store>
{
  public:
    X509Store();
    X509Store(X509_STORE* raw, bool acquire) noexcept
      : ObjectHolder(raw, acquire)
    {
    }
    X509Store(const X509Store& other) = delete;
    X509Store(X509Store&& other) = default;

    X509Store& operator=(const X509Store& other) = delete;
    X509Store& operator=(X509Store&& other) noexcept
    {
        ObjectHolder::operator=(std::move(other));
        return *this;
    }

    ~X509Store() = default;
    static void destroy(X509_STORE* raw) noexcept;

    void loadFile(const std::string& path);
    void loadDirectory(const std::string& path);
    void loadDefaultLocation();

    void setTrust(bool flag);
};
