#pragma once

#include "utils/ObjectHolder.h"

#include <openssl/x509v3.h>

#include <string>
#include <vector>

class DistPoint : ObjectHolder<DIST_POINT, DistPoint>
{
    friend class ObjectHelper;

  public:
    typedef DIST_POINT RawType;

    DistPoint(DIST_POINT* point, bool acquire)
      : ObjectHolder(point, acquire)
    {
    }

    DistPoint(DistPoint&& other) = default;

    DistPoint(const DistPoint&) = delete;
    DistPoint& operator=(const DistPoint&) = delete;

    static void destroy(DIST_POINT* raw);

    std::vector<std::string> get_distribution_point_names();
    std::vector<std::string> get_crl_issuers();
};
