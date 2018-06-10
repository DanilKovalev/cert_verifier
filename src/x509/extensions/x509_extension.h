#pragma once

#include <openssl/x509.h>

class x509_extension
{
public:
    x509_extension(X509_EXTENSION* ext, bool acquire);

    x509_extension(const x509_extension& rhs);
    x509_extension(x509_extension&& rhs) noexcept;
    ~x509_extension() noexcept;

    x509_extension& operator =(const x509_extension& rhs);
    x509_extension& operator =(x509_extension&& rhs) noexcept;

    X509_EXTENSION* raw();
    const X509_EXTENSION* raw() const;

    friend void swap(x509_extension& a, x509_extension& b) noexcept;
    bool is_critical() const noexcept ;
    int nid() const noexcept ;

    static x509_extension create_wrapper(X509_EXTENSION* ext);
    static x509_extension create_attached(X509_EXTENSION* ext);

private:

    static X509_EXTENSION* duplicate(X509_EXTENSION *pExt);
    void free() noexcept;

private:
    X509_EXTENSION* m_ext;
    bool m_acquired;
};

void swap(x509_extension& a, x509_extension& b) noexcept;
