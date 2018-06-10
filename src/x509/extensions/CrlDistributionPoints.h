#pragma once

#include "X509Extension.h"
#include "DistPoint.h"
#include <openssl/x509v3.h>

#include <string>

class CrlDistributionPoints : public X509Extension
{
public:
    explicit CrlDistributionPoints(X509Extension& ext);
    CrlDistributionPoints(const CrlDistributionPoints&) = delete;
    CrlDistributionPoints(CrlDistributionPoints&& ) noexcept;
    CrlDistributionPoints& operator=(const CrlDistributionPoints& ) = delete;

    ~CrlDistributionPoints() override;

    int size() const noexcept;
    bool empty() const noexcept;
    DistPoint operator[](int);

    friend bool operator == (const CrlDistributionPoints& lhs,
                             const CrlDistributionPoints& rhs) noexcept;

private:
    CRL_DIST_POINTS* m_points;
};


