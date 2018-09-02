#include "DistPoint.h"
#include "../../SslException.h"
#include "../../utils/general_name.h"

#include <openssl/x509v3.h>
#include <boost/numeric/conversion/cast.hpp>
#include <vector>


DistPoint::DistPoint(DIST_POINT *point)
: m_point(point)
{
}

DistPoint::DistPoint(DistPoint && rhs) noexcept
: m_point(std::exchange(rhs.m_point, nullptr))
{
}

DistPoint::~DistPoint() = default;

std::vector<std::string> DistPoint::get_distribution_point_names()
{
    std::vector<std::string> result;
    if(m_point->distpoint == nullptr || m_point->distpoint->type != 0)
        return result;

    GENERAL_NAMES* names = m_point->distpoint->name.fullname;
    for(int i=0; i < sk_GENERAL_NAME_num(names); ++i)
    {
        GENERAL_NAME* name = sk_GENERAL_NAME_value(names, i);
        result.push_back(to_string(name));
    }

    return result;
}

std::vector<std::string> DistPoint::get_crl_issuers()
{
    std::vector<std::string> result;
    if(!m_point->CRLissuer)
        return result;

    for(int i = 0; i < sk_GENERAL_NAME_num(m_point->CRLissuer); ++i)
    {
        GENERAL_NAME* name = sk_GENERAL_NAME_value(m_point->CRLissuer, i);
        result.push_back(to_string(name));
    }

    return result;
}
