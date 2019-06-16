#include "AuthorityInformationAccess.h"

#include "../../SslException.h"
#include "../../bio/bio_ostring.h"
#include "../../utils/general_name.h"

#include <boost/numeric/conversion/cast.hpp>

#include <openssl/x509v3.h>

AuthorityInformationAccess::AuthorityInformationAccess(const X509Extension& ext)
  : X509Extension(ext)
  , m_oscp()
  , m_caIssuer()
{
    if (ext.nid() != NID)
        throw std::runtime_error("Wrong extension nid type");

    parse();
}

AuthorityInformationAccess::AuthorityInformationAccess(X509Extension&& ext)
  : X509Extension(std::move(ext))
{
    if (ext.nid() != NID)
        throw std::runtime_error("Wrong extension nid type");

    parse();
}

void AuthorityInformationAccess::parse()
{
    auto access = static_cast<AUTHORITY_INFO_ACCESS*>(::X509V3_EXT_d2i(m_raw));
    for (int i = 0; i < sk_ACCESS_DESCRIPTION_num(access); ++i)
    {
        ACCESS_DESCRIPTION* desc = sk_ACCESS_DESCRIPTION_value(access, i);
        if (desc->location->type != GEN_URI)
            throw SslException("location->type != GEN_URI");

        std::string uri = to_string(desc->location);

        int nid = OBJ_obj2nid(desc->method);
        if (nid == NID_ad_OCSP)
            m_oscp = uri;
        else if (nid == NID_ad_ca_issuers)
            m_caIssuer = uri;
        else
            throw std::runtime_error("Unknown method in AuthorityInformationAccess");
    }
    ::AUTHORITY_INFO_ACCESS_free(access);
}

const std::string& AuthorityInformationAccess::oscp() const
{
    return m_oscp;
}

const std::string& AuthorityInformationAccess::ca_issuer() const
{
    return m_caIssuer;
}
