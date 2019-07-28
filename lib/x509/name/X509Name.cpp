#include "X509Name.h"

#include "SslException.h"

#include <iostream>

std::string X509Name::toString() const
{
    return std::to_string(m_raw);
}

std::optional<X509NameEntry> X509Name::findEntry(int nid)
{
    int idx = X509_NAME_get_index_by_NID(m_raw, nid, -1);
    if (idx == -1)
        return std::nullopt;

    return ObjectHelper<X509NameEntry>::makeCopied(X509_NAME_get_entry(m_raw, idx));
}

X509NameEntry X509Name::getEntry(int nid)
{
    std::optional<X509NameEntry> entry = findEntry(nid);
    if (!entry.has_value())
        throw SslException("X509 name entry not found");

    return entry.value();
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
    char* pName = X509_NAME_oneline(raw, nullptr, 0);
    if (!pName)
        throw SslException("X509_NAME_oneline");

    std::string result(pName);
    free(pName);

    return result;
}
