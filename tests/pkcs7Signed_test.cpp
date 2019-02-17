#include "pkcs/Pkcs7.h"
#include "pkcs/Pkcs7Signed.h"
#include "SslException.h"

#include <catch2/catch.hpp>
#include <iostream>
#include "utils.h"
#include "template_tests.h"

TEST_CASE( "pkcs7signed get content", "[pkcs7]")
{
    auto der = read_binary_file("content/dstrootcax3.p7c");
    Pkcs7 pkcs7 = Pkcs7::fromDer(der);

    Pkcs7Signed pkcs7Signed (std::move(pkcs7));
    CHECK(pkcs7Signed.getCertificates().size() != 0);
}



