#include <catch.hpp>
#include "utils.h"

#include "private_key.h"

TEST_CASE( "Private key read", "[priv_key]" )
{
    std::string path = "content/";
    SECTION("key")
        path += "key.pem";

    std::string pem = read_file(path);
    REQUIRE_NOTHROW(private_key::from_pem(pem));
}

