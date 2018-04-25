#include "dist_point.h"
#include "../ssl_exc.h"

#include <openssl/x509v3.h>
#include <boost/numeric/conversion/cast.hpp>
#include <vector>


dist_point::dist_point(DIST_POINT *point)
: m_point(point)
{
}

dist_point::dist_point(dist_point && rhs)
: m_point(std::exchange(rhs.m_point, nullptr))
{
}

dist_point::~dist_point()
{
}

std::vector<std::string> dist_point::get_distribution_point_names()
{
    std::vector<std::string> result;
    if(m_point->distpoint == nullptr || m_point->distpoint->type != 0)
        return result;

    GENERAL_NAMES* names = m_point->distpoint->name.fullname;
    for(int i=0; i < sk_GENERAL_NAME_num(names); ++i)
    {
        GENERAL_NAME* name = sk_GENERAL_NAME_value(names, i);
        if (name->type != GEN_URI)
            throw ssl_exc("type != GEN_URI");

        unsigned char* symbols = ASN1_STRING_data(name->d.uniformResourceIdentifier);
        int length = ASN1_STRING_length(name->d.uniformResourceIdentifier);
        std::string uri(reinterpret_cast<char*>(symbols), boost::numeric_cast<size_t >(length));
        result.push_back(uri);
    }

    return result;
}

std::vector<std::string> dist_point::get_crl_issuers()
{
    std::vector<std::string> result;
    if(!m_point->CRLissuer)
        return result;

    for(int i = 0; i < sk_GENERAL_NAME_num(m_point->CRLissuer); ++i)
    {
        GENERAL_NAME* name = sk_GENERAL_NAME_value(m_point->CRLissuer, i);
        if (name->type != GEN_URI)
            throw ssl_exc("type != GEN_URI");

        unsigned char* symbols = ASN1_STRING_data(name->d.uniformResourceIdentifier);
        int length = ASN1_STRING_length(name->d.uniformResourceIdentifier);
        std::string uri(reinterpret_cast<char*>(symbols), boost::numeric_cast<size_t >(length));
        result.push_back(uri);
    }

    return result;
}
