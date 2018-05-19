#include "crl_distribution_points.h"
#include "../SslException.h"

#include <boost/numeric/conversion/cast.hpp>
#include <vector>


crl_distribution_points::crl_distribution_points(x509_extension &ext)
 : x509_extension(ext.raw(), false)
 , m_points(nullptr)
{
    m_points = static_cast<CRL_DIST_POINTS*>(X509V3_EXT_d2i( raw() ));
    if (!m_points)
        throw SslException("X509V3_EXT_d2i for CRL_DIST_POINTS");
}

crl_distribution_points::~crl_distribution_points()
{
    if(m_points)
        CRL_DIST_POINTS_free(m_points);

    m_points = nullptr;
}

crl_distribution_points::crl_distribution_points(crl_distribution_points &&rhs) noexcept
: x509_extension(std::move(rhs))
, m_points(std::exchange(rhs.m_points, nullptr))
{

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

bool operator==(const crl_distribution_points &lhs, const crl_distribution_points &rhs) noexcept
{
    return lhs.m_points == rhs.m_points;
}

bool crl_distribution_points::empty() const noexcept
{
    return static_cast<bool>(size());
}
