#include "X509Object.h"

#include "utils/ObjectHelper.h"

X509Object::X509Object()
  : ObjectHolder(X509_OBJECT_new(), true)
{
    if (!m_raw)
        throw SslException("X509_OBJECT_new");
}

void X509Object::destroy(X509_OBJECT* raw) noexcept
{
    X509_OBJECT_free(raw);
}
X509_LOOKUP_TYPE X509Object::getType() const noexcept
{
    return X509_OBJECT_get_type(m_raw);
}

X509Certificate X509Object::toX509Certificate() const
{
    if (getType() != X509_LU_X509)
        throw std::runtime_error("Wrong X5009_OBJECT type to converting to X509_Certificate");
    return ObjectHelper<X509Certificate>::makeCopied(X509_OBJECT_get0_X509(m_raw));
}
