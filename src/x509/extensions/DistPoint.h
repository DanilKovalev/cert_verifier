#pragma once

#include <openssl/x509v3.h>
#include <string>
#include <vector>

class DistPoint
{
public:
    explicit DistPoint(DIST_POINT* point);
    DistPoint(DistPoint&& ) noexcept;
    DistPoint(const DistPoint& ) = delete;
    DistPoint& operator=(const DistPoint& ) = delete;
    ~DistPoint();

    std::vector<std::string> get_distribution_point_names();
    std::vector<std::string> get_crl_issuers();

private:
    DIST_POINT* m_point;
};


