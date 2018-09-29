#pragma once

#include <openssl/x509v3.h>
#include <string>
#include <vector>

class DistPoint
{
public:
    typedef DIST_POINT RawType;

    DistPoint(DIST_POINT* point, bool acquire) noexcept;
    DistPoint(DistPoint&& ) noexcept;
    DistPoint(const DistPoint& ) = delete;
    DistPoint& operator=(const DistPoint& ) = delete;
    ~DistPoint();

    std::vector<std::string> get_distribution_point_names();
    std::vector<std::string> get_crl_issuers();

private:
    DIST_POINT* m_point;
    bool m_acquired;
};


