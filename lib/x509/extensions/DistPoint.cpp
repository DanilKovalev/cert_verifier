#include "DistPoint.h"

#include "../../SslException.h"
#include "../../utils/X509Name.h"
#include "../../utils/general_name.h"

#include <boost/numeric/conversion/cast.hpp>

#include <openssl/x509v3.h>

#include <vector>

std::vector<std::string> DistPoint::get_distribution_point_names()
{
    std::vector<std::string> result;
    if (m_raw->distpoint == nullptr)
        return result;

    if (m_raw->distpoint->type == 0)
    {
        GENERAL_NAMES* names = m_raw->distpoint->name.fullname;

        for (int i = 0; i < sk_GENERAL_NAME_num(names); ++i)
        {
            GENERAL_NAME* name = sk_GENERAL_NAME_value(names, i);
            result.push_back(to_string(name));
        }
    }
    else if (m_raw->distpoint->type == 1)
    {
        result.push_back(std::to_string(m_raw->distpoint->dpname));
    }
    else
        throw std::runtime_error("Unknown distpoint type");

    return result;
}

std::vector<std::string> DistPoint::get_crl_issuers()
{
    std::vector<std::string> result;
    if (!m_raw->CRLissuer)
        return result;

    for (int i = 0; i < sk_GENERAL_NAME_num(m_raw->CRLissuer); ++i)
    {
        GENERAL_NAME* name = sk_GENERAL_NAME_value(m_raw->CRLissuer, i);
        result.push_back(to_string(name));
    }

    return result;
}

void DistPoint::destroy(DIST_POINT* raw)
{
    DIST_POINT_free(raw);
}
