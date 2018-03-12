#include <catch.hpp>
#include <fstream>
#include <iostream>

#include "private_key.h"

TEST_CASE( "Private key read", "[priv_key]" )
{
    std::string path = "content/";
    SECTION("key")
        path += "key.pem";

    std::ifstream file(path);
    if (!file.is_open())
        std::__throw_system_error(errno);

    std::string pem((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    REQUIRE_NOTHROW(private_key::from_pem(pem));
}

