#include <catch2/catch.hpp>
#include "utils.h"
#include "template_tests.h"

#include "PrivateKey.h"

TEST_CASE( "Private key read", "[priv_key]" )
{
    std::string path = "content/key.pem";

    std::string pem = read_file(path);

    CHECK_THROWS_AS(PrivateKey::from_pem("rubbish"), SslException);
    PrivateKey privateKey = PrivateKey::from_pem(pem);
    CHECK(privateKey.to_pem() == pem);

    const PrivateKey& cref = privateKey;
    CHECK(cref.raw() != nullptr);
    CHECK(privateKey.raw() != nullptr);

    CHECK_NOTHROW(move_test(privateKey));
}

