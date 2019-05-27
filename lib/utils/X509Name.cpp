#include "X509Name.h"

#include "ObjectHelper.h"
#include "SslException.h"

#include <memory>

std::string X509Name::toString() const
{
    return std::to_string(m_raw);
}

X509_NAME* X509Name::duplicate(X509_NAME* raw)
{
    X509_NAME* result = X509_NAME_dup(raw);
    if (result == nullptr)
        throw SslException("Failed to duplicate X509 Name");

    return result;
}

void X509Name::destroy(X509_NAME* raw) noexcept
{
    X509_NAME_free(raw);
}

std::string std::to_string(const X509_NAME* raw)
{
    std::unique_ptr<char> ptrName(X509_NAME_oneline(raw, nullptr, 0));
    if (!ptrName)
        throw SslException("X509_NAME_oneline");

    return ptrName.get();
}
