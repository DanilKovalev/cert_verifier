#include "AuthorityKeyId.h"

#include <openssl/x509v3.h>

AuthorityKeyId::AuthorityKeyId(const X509Extension& ext)
  : X509Extension(ext)
{
    if (ext.nid() != NID)
        throw std::runtime_error("Wrong extension nid type");
}

AuthorityKeyId::AuthorityKeyId(X509Extension&& ext)
  : X509Extension(std::move(ext))
{
    if (ext.nid() != NID)
        throw std::runtime_error("Wrong extension nid type");
}
