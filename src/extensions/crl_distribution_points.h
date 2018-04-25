#pragma once

#include "../x509_extension.h"
#include "dist_point.h"
#include <openssl/x509v3.h>

#include <string>

class crl_distribution_points : public x509_extension
{
public:
    explicit crl_distribution_points(x509_extension& ext);

    int size() const noexcept;
    dist_point operator[](int);

private:
    CRL_DIST_POINTS* m_points;
};


