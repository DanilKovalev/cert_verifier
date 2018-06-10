#include <catch.hpp>
#include "utils.h"

#include "PrivateKey.h"

TEST_CASE( "Private key read", "[priv_key]" )
{
    std::string path = "content/";
    SECTION("key")
        path += "key.pem";

    std::string pem = read_file(path);
    REQUIRE_NOTHROW(PrivateKey::from_pem(pem));
}

