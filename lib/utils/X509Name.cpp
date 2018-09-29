#include "X509Name.h"

#include <memory>

#include "utils.h"
#include "SslException.h"

X509Name::X509Name(X509_NAME* raw, bool acquire) noexcept
: m_raw(raw)
, m_acquired(acquire)
{
}

std::string X509Name::string() const
{
    return std::to_string(m_raw);
}

std::string std::to_string(const X509_NAME* raw)
{
    char* name = X509_NAME_oneline(raw, nullptr, 0);
    if (name == nullptr)
        throw SslException("X509_NAME_oneline");

    auto holder = makeObjectHolder(name);
    return name;
}

