#include "CrlDistributionPoints.h"
#include "../../SslException.h"

#include <boost/numeric/conversion/cast.hpp>
#include <vector>


CrlDistributionPoints::CrlDistributionPoints(X509Extension &ext)
 : X509Extension(ext.raw(), false)
 , m_points(nullptr)
{
    m_points = static_cast<CRL_DIST_POINTS*>(X509V3_EXT_d2i( raw() ));
    if (!m_points)
        throw SslException("X509V3_EXT_d2i for CRL_DIST_POINTS");
}

CrlDistributionPoints::~CrlDistributionPoints()
{
    if(m_points)
        CRL_DIST_POINTS_free(m_points);

    m_points = nullptr;
}

CrlDistributionPoints::CrlDistributionPoints(CrlDistributionPoints &&rhs) noexcept
: X509Extension(std::move(rhs))
, m_points(std::exchange(rhs.m_points, nullptr))
{

}

int CrlDistributionPoints::size() const noexcept
{
    return sk_DIST_POINT_num(m_points);
}

DistPoint CrlDistributionPoints::operator[](int idx)
{
    DIST_POINT* point = sk_DIST_POINT_value(m_points, idx);
    return DistPoint(point);
}

bool operator==(const CrlDistributionPoints &lhs, const CrlDistributionPoints &rhs) noexcept
{
    return lhs.m_points == rhs.m_points;
}

bool CrlDistributionPoints::empty() const noexcept
{
    return static_cast<bool>(size());
}
