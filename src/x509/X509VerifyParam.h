#pragma once
#include <openssl/x509_vfy.h>
#include <string>

class X509VerifyParam
{
public:
    X509VerifyParam();
    X509VerifyParam(X509_VERIFY_PARAM* raw, bool acquire) noexcept;
    X509VerifyParam(const X509VerifyParam& other) = delete;
    X509VerifyParam(X509VerifyParam&& other);
    X509VerifyParam& operator=(X509VerifyParam&& other) noexcept;
    X509VerifyParam& operator=(const X509VerifyParam& other)= delete;
    ~X509VerifyParam();

    void swap(X509VerifyParam& other) noexcept;

    X509_VERIFY_PARAM* raw() noexcept;
    const X509_VERIFY_PARAM* raw() const noexcept;
    X509_VERIFY_PARAM* detach();

    void setHost(const std::string& host);
    void setDepth(int depth) noexcept;
    
private:
    void free() noexcept;
private:
    X509_VERIFY_PARAM* m_raw;
    bool m_acquired;
};

namespace std
{
    template <>
    inline void swap(X509VerifyParam& a, X509VerifyParam& b) noexcept
    {
        a.swap(b);
    }
}
