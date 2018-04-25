#pragma once

#include <openssl/x509v3.h>
#include <string>
#include <vector>

class dist_point
{
public:
    explicit dist_point(DIST_POINT* point);
    dist_point(dist_point&& );
    dist_point(const dist_point& ) = delete;
    dist_point& operator=(const dist_point& ) = delete;
    ~dist_point();

    std::vector<std::string> get_distribution_point_names();
    std::vector<std::string> get_crl_issuers();

private:
    DIST_POINT* m_point;
};


