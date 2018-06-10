#pragma once

#include <openssl/x509_vfy.h>

#include <string>

class X509Store
{
public:
    X509Store();
    X509Store(X509_STORE* store, bool acquire) noexcept;
    X509Store(const X509Store& other) = delete;
    X509Store(X509Store&& other);
    X509Store& operator =(X509Store&& other) noexcept;
    X509Store& operator =(const X509Store& other) = delete;

    ~X509Store();

    void swap(X509Store& other) noexcept;

    X509_STORE* raw();
    const X509_STORE* raw() const;

    void loadFile(const std::string& path);
    void loadDirectory(const std::string& path);
    void loadDefaultLocation();

    void setTrust(bool flag);

private:
    void free() noexcept;

private:
    X509_STORE *m_store;
    bool m_acquired;
};

namespace std
{
    template <>
    inline void swap(X509Store& a, X509Store& b) noexcept
    {
        a.swap(b);
    }
}
