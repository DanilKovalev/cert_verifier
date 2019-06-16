#include "CrlDistributionPoints.h"

#include "../../SslException.h"

#include <boost/numeric/conversion/cast.hpp>

#include <vector>

CrlDistributionPoints::CrlDistributionPoints(X509Extension&& ext)
  : X509Extension(std::move(ext))
{
    if (ext.nid() != NID)
        throw std::runtime_error("Wrong extension nid type");
}

CrlDistributionPoints::CrlDistributionPoints(const X509Extension& ext)
  : X509Extension(ext)
{
    if (ext.nid() != NID)
        throw std::runtime_error("Wrong extension nid type");
}

StackOf<DistPoint> CrlDistributionPoints::getDistPoints()
{
    auto* points = static_cast<CRL_DIST_POINTS*>(X509V3_EXT_d2i( m_raw ));
    if (!points)
        throw SslException("X509V3_EXT_d2i for CRL_DIST_POINTS");

    return StackOf<DistPoint>(reinterpret_cast<struct stack_st*>(points), true);
}
