#include "SslException.h"

#include <openssl/err.h>
#include <sstream>
#include <boost/numeric/conversion/cast.hpp>

namespace
{
    struct ssl_error_category : std::error_category
    {
        ~ssl_error_category() override = default;

        const char* name() const noexcept override
        {
            return "ssl";
        }

        std::string message(int ec) const override
        {
            return ERR_error_string(boost::numeric_cast<ulong>(ec), nullptr);
        }
    };
}

const std::error_category& ssl_category() noexcept
{
    static ssl_error_category instance;
    return instance;
}

SslException::SslException(const char* what)
: SslException(boost::numeric_cast<int>(ERR_get_error()), what)
{
}



