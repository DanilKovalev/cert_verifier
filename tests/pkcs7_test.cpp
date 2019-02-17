#include "pkcs/Pkcs7.h"
#include "SslException.h"

#include <catch2/catch.hpp>
#include <iostream>
#include "utils.h"
#include "template_tests.h"


TEST_CASE( "pkcs7 read", "[pkcs7][der]")
{
    auto der = read_binary_file("content/dstrootcax3.p7c");
    Pkcs7 pkcs7 = Pkcs7::fromDer(der);
    REQUIRE(der == pkcs7.toDer());

}


