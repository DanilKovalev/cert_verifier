#include "Pkcs12.h"

#include "bio/bio_guard.h"
#include "ssl_exc.h"

#include <boost/numeric/conversion/cast.hpp>

#include <stdexcept>

Pkcs12::Pkcs12(PKCS12* pkcs12, bool acquire)
: m_pkcs12(pkcs12)
, m_acquired(acquire)
{

}

Pkcs12 Pkcs12::from_der(const uint8_t *bytes, size_t size)
{
    PKCS12* pkcs12;
    if (d2i_PKCS12(&pkcs12, &bytes, boost::numeric_cast<long>(size)) == nullptr)
        throw ssl_exc("d2i_PKCS12");

    return Pkcs12(pkcs12, true);
}
