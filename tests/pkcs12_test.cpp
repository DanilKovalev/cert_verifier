#include "Pkcs12.h"
#include "SslException.h"

#include <iostream>
#include <catch.hpp>
#include <openssl/err.h>
#include "utils.h"
#include "template_tests.h"


TEST_CASE( "Pkcs12 read", "[pkcs12]" )
{
    std::string path = "content/";
    SECTION("firefox example")
        path += "1571753451.p12";

    auto der = read_binary_file(path);
    Pkcs12 pkcs12 = Pkcs12::fromDer(der.data(), der.size());
}

TEST_CASE( "Pkcs12 enc", "[pkcs12]" )
{
    std::string path = "content/";
    SECTION("firefox example")
        path += "1571753451.p12";

    auto der = read_binary_file(path);
    Pkcs12 pkcs12 = Pkcs12::fromDer(der.data(), der.size());
    pkcs12.changePassword("newPwd", "test");

    REQUIRE_THROWS_AS(pkcs12.parse("wrong pwd"), SslException);
    REQUIRE_NOTHROW(pkcs12.parse ("newPwd"));
}

TEST_CASE( "Pkcs12 create", "[pkcs12]" )
{
    std::string path = "content/";
    SECTION("firefox example")
        path += "1571753451.p12";

    auto der = read_binary_file(path);
    Pkcs12 pkcs12 = Pkcs12::fromDer(der.data(), der.size());
    Pkcs12Data data = pkcs12.parse("test");

    REQUIRE_NOTHROW(pkcs12 = Pkcs12::create(data, ""));
    REQUIRE_NOTHROW(data =  pkcs12.parse(""));
}

TEST_CASE( "pkcs12 memory test", "[pkcs12]")
{
    std::string path = "content/";
    SECTION("firefox example")
        path += "1571753451.p12";

    auto der = read_binary_file(path);
    Pkcs12 pkcs12 = Pkcs12::fromDer(der.data(), der.size());

    REQUIRE_NOTHROW(move_test(pkcs12));
}



