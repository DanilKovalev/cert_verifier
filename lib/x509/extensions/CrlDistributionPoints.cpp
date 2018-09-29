#include "CrlDistributionPoints.h"
#include "../../SslException.h"

#include <boost/numeric/conversion/cast.hpp>
#include <vector>


CrlDistributionPoints::CrlDistributionPoints(CRL_DIST_POINTS* raw, bool acquire)
: StackOf(reinterpret_cast<struct stack_st*>(raw), acquire)
{
}

CrlDistributionPoints::CrlDistributionPoints(CrlDistributionPoints&& other) noexcept
: StackOf(std::move(other))
{
}

CrlDistributionPoints& CrlDistributionPoints::operator=(CrlDistributionPoints&& other) noexcept
{
    if (this == &other)
        return *this;

    StackOf::operator=(std::move(other));
    return *this;
}

CrlDistributionPoints CrlDistributionPoints::fromExtension(X509Extension& ext)
{
    auto* points = static_cast<CRL_DIST_POINTS*>(X509V3_EXT_d2i( ext.raw() ));
    if (!points)
        throw SslException("X509V3_EXT_d2i for CRL_DIST_POINTS");

    return CrlDistributionPoints(points, true);
}

