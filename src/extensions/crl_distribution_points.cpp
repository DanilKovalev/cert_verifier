#include "crl_distribution_points.h"
#include "../ssl_exc.h"

#include <boost/numeric/conversion/cast.hpp>
#include <vector>


crl_distribution_points::crl_distribution_points(x509_extension &ext)
 : x509_extension(ext.raw(), false)
{
    m_points = static_cast<CRL_DIST_POINTS*>(X509V3_EXT_d2i( raw() ));
    if (!m_points)
        throw ssl_exc("X509V3_EXT_d2i for CRL_DIST_POINTS");
}

int crl_distribution_points::size() const noexcept
{
    return sk_DIST_POINT_num(m_points);
}

dist_point crl_distribution_points::operator[](int idx)
{
    DIST_POINT* point = sk_DIST_POINT_value(m_points, idx);
    return dist_point(point);
}
