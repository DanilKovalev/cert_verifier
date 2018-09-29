#include "DistPoint.h"
#include "../../SslException.h"
#include "../../utils/general_name.h"
#include "../../utils/X509Name.h"

#include <openssl/x509v3.h>
#include <boost/numeric/conversion/cast.hpp>
#include <vector>


DistPoint::DistPoint(DIST_POINT *point, bool acquire) noexcept
 : m_point(point)
 , m_acquired(acquire)
{
}

DistPoint::DistPoint(DistPoint && rhs) noexcept
 : m_point(std::exchange(rhs.m_point, nullptr))
 , m_acquired(std::exchange(rhs.m_acquired, false))
{
}

DistPoint::~DistPoint()
{
    if(!m_acquired)
        return;

    DIST_POINT_free(m_point);
    m_acquired = false;
    m_point = nullptr;
}

std::vector<std::string> DistPoint::get_distribution_point_names()
{
    std::vector<std::string> result;
    if(m_point->distpoint == nullptr )
        return result;

    if(m_point->distpoint->type == 0)
    {
        GENERAL_NAMES* names = m_point->distpoint->name.fullname;

        for(int i=0; i < sk_GENERAL_NAME_num(names); ++i)
        {
            GENERAL_NAME* name = sk_GENERAL_NAME_value(names, i);
            result.push_back(to_string(name));
        }
    }
    else if (m_point->distpoint->type == 1)
    {
        result.push_back(std::to_string(m_point->distpoint->dpname));
    }
    else
        throw std::runtime_error("Unknown distpoint type");

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
