#pragma once

#include "X509Extension.h"
#include "DistPoint.h"
#include "utils/StackOf.h"

#include <string>
#include <openssl/x509v3.h>

class CrlDistributionPoints : public X509Extension
{
public:
    static const int NID = NID_crl_distribution_points;

    explicit CrlDistributionPoints(const X509Extension& ext);
    explicit CrlDistributionPoints(X509Extension&& ext);

    CrlDistributionPoints(const CrlDistributionPoints&) = default;
    CrlDistributionPoints(CrlDistributionPoints&&) = default;

    ~CrlDistributionPoints() = default;

    StackOf<DistPoint> getDistPoints();
};


