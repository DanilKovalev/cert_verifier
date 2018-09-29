#pragma once

#include "X509Extension.h"
#include "DistPoint.h"
#include "utils/StackOf.h"

#include <string>
#include <openssl/x509v3.h>

class CrlDistributionPoints : public StackOf<DistPoint>
{
public:
    CrlDistributionPoints(CRL_DIST_POINTS* raw, bool acquire);
    CrlDistributionPoints(CrlDistributionPoints&& other) noexcept;

    CrlDistributionPoints& operator =(CrlDistributionPoints&& other) noexcept;

    ~CrlDistributionPoints() override = default;

    static CrlDistributionPoints fromExtension(X509Extension &ext);
};


