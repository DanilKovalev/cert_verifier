#include <catch.hpp>
#include "utils.h"

#include "x509_certificate.h"

TEST_CASE( "Cert read", "[cert]" )
{
    std::string path = "content/";
    SECTION("cert")
        path += "cert.pem";

    SECTION("telegramorg")
        path += "telegramorg.crt";

    SECTION("toxchat.crt")
        path += "toxchat.crt";

    std::string pem = read_file(path);
    REQUIRE_NOTHROW(x509_certificate::from_pem(pem));
}

