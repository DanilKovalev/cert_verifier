#include "Pkcs12.h"
#include "SslException.h"

#include <iostream>
#include <catch.hpp>
#include <openssl/err.h>
#include "utils.h"


TEST_CASE( "Pkcs12 read", "[pkcs12]" )
{
    std::string path = "content/";
    SECTION("firefox example")
        path += "1571753451.p12";

    auto der = read_binary_file(path);
    Pkcs12 pkcs12 = Pkcs12::fromDer(der.data(), der.size());

    std::vector<x509_certificate> additional;

    REQUIRE_THROWS_AS(pkcs12.parse("test1", additional), SslException);
    REQUIRE_NOTHROW(pkcs12.parse("test", additional));
}




