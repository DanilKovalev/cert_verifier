#include "PKCS12_container.h"

#include "bio/bio_guard.h"
#include "ssl_exc.h"

#include <boost/numeric/conversion/cast.hpp>

#include <stdexcept>


PKCS12_container::PKCS12_container(PKCS12 *pRaw)
 : m_pRaw(pRaw)
{
}

PKCS12_container PKCS12_container::from_der(const std::vector<uint8_t>& der)
{
    auto len = boost::numeric_cast<int>(der.size());
    auto bio = create_bio_guard(BIO_new_mem_buf(der.data(), len));

    PKCS12* pRaw;
    if ( d2i_PKCS12_bio(bio.get(), &pRaw))
        throw ssl_exc("d2i_PKCS12_bio failed");

    return PKCS12_container(pRaw);
}
