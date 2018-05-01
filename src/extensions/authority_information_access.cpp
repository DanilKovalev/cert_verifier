#include "authority_information_access.h"

#include "../ssl_exc.h"
#include "../utils/general_name.h"
#include "../bio/bio_ostring.h"

#include <boost/numeric/conversion/cast.hpp>
#include <openssl/x509v3.h>


authority_information_access::authority_information_access(x509_extension &ext)
 : x509_extension(ext.raw(), false)
 , m_oscp()
 , m_ca_issuers()
{
    parse();
}

void authority_information_access::parse()
{
    auto access = static_cast<AUTHORITY_INFO_ACCESS*>(::X509V3_EXT_d2i(raw()));
    for (int i = 0; i < sk_ACCESS_DESCRIPTION_num(access); ++i)
    {
        ACCESS_DESCRIPTION *desc = sk_ACCESS_DESCRIPTION_value(access, i);
        if (desc->location->type != GEN_URI)
            throw ssl_exc("location->type != GEN_URI");

        std::string uri = to_string(desc->location);

        int nid = OBJ_obj2nid(desc->method);
        if (nid == NID_ad_OCSP)
            m_oscp = uri;
        else if(nid == NID_ad_ca_issuers)
            m_ca_issuers = uri;
        else
            throw std::runtime_error("Unknown method in authority_information_access");
    }
    ::AUTHORITY_INFO_ACCESS_free(access);
}

const std::string &authority_information_access::oscp() const
{
    return  m_oscp;
}

const std::string &authority_information_access::ca_issuer() const
{
    return m_ca_issuers;
}

