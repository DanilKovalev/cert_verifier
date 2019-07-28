#include "X509NameEntry.h"

#include "SslException.h"
#include "utils/Asn1String.h"

X509NameEntry::RawType* X509NameEntry::duplicate(X509NameEntry::RawType* raw)
{
    RawType* result = X509_NAME_ENTRY_dup(raw);
    if (result == nullptr)
        throw SslException("Failed to duplicate X509_NAME_ENTRY");

    return result;
}

void X509NameEntry::destroy(X509NameEntry::RawType* raw) noexcept
{
    X509_NAME_ENTRY_free(raw);
}

std::string X509NameEntry::toString() const
{
    return Asn1String::ToUtf8(X509_NAME_ENTRY_get_data(m_raw));
}

int X509NameEntry::nid() const noexcept
{
    ASN1_OBJECT* obj = X509_NAME_ENTRY_get_object(m_raw);
    return OBJ_obj2nid(obj);
}
