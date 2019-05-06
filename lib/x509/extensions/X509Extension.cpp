#include "X509Extension.h"
#include "../../SslException.h"
#include <openssl/x509v3.h>
#include <utility>
#include <memory>
#include <openssl/pkcs12.h>


X509_EXTENSION *X509Extension::duplicate(X509_EXTENSION *pExt)
{
    X509_EXTENSION* result = X509_EXTENSION_dup(pExt);
    if(result == nullptr)
        throw SslException("Failed to duplicate X509 certificate");

    return result;
}

void X509Extension::destroy(RawType* raw) noexcept
{
    X509_EXTENSION_free(raw);
}


bool X509Extension::is_critical() const noexcept
{
    return static_cast<bool>(X509_EXTENSION_get_critical(m_raw));
}

int X509Extension::nid() const noexcept
{
    ASN1_OBJECT *obj = X509_EXTENSION_get_object(m_raw);
    return OBJ_obj2nid(obj);
}


