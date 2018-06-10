#pragma once

#include "x509_extension.h"
#include "dist_point.h"
#include <openssl/x509v3.h>

#include <string>

class crl_distribution_points : public x509_extension
{
public:
    explicit crl_distribution_points(x509_extension& ext);
    crl_distribution_points(const crl_distribution_points&) = delete;
    crl_distribution_points(crl_distribution_points&& ) noexcept;
    crl_distribution_points& operator=(const crl_distribution_points& ) = delete;

    ~crl_distribution_points();

    int size() const noexcept;
    bool empty() const noexcept;
    dist_point operator[](int);

    friend bool operator == (const crl_distribution_points& lhs,
                             const crl_distribution_points& rhs) noexcept;

private:
    CRL_DIST_POINTS* m_points;
};


