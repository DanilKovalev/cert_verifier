#include "SslVerifyException.h"

#include <openssl/err.h>
#include <openssl/x509.h>
#include <boost/numeric/conversion/cast.hpp>

namespace
{
    struct ssl_verify_error_category : std::error_category
    {
        ~ssl_verify_error_category() override = default;

        const char* name() const noexcept override
        {
            return "ssl verify";
        }

        std::string message(int ec) const override
        {
            return X509_verify_cert_error_string(static_cast<long>(ec));
        }
    };
}



const std::error_category& ssl_verify_category() noexcept
{
    static ssl_verify_error_category instance;
    return instance;
}
