#pragma once

#include <openssl/x509.h>

class X509Extension
{
public:
    typedef X509_EXTENSION RawType;
    
    X509Extension(X509_EXTENSION* ext, bool acquire);

    X509Extension(const X509Extension& rhs);
    X509Extension(X509Extension&& rhs) noexcept;
    virtual ~X509Extension() noexcept;

    X509Extension& operator =(const X509Extension& rhs);
    X509Extension& operator =(X509Extension&& rhs) noexcept;

    X509_EXTENSION* raw();
    const X509_EXTENSION* raw() const;

    friend void swap(X509Extension& a, X509Extension& b) noexcept;
    bool is_critical() const noexcept ;
    int nid() const noexcept ;

    static X509Extension create_wrapper(X509_EXTENSION* ext);
    static X509Extension create_attached(X509_EXTENSION* ext);

private:

    static X509_EXTENSION* duplicate(X509_EXTENSION *pExt);
    void free() noexcept;

private:
    X509_EXTENSION* m_ext;
    bool m_acquired;
};

void swap(X509Extension& a, X509Extension& b) noexcept;
