#include <catch.hpp>
#include <fstream>
#include <iostream>

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

    std::ifstream file(path);
    if (!file.is_open())
        std::__throw_system_error(errno);

    std::string pem((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    REQUIRE_NOTHROW(x509_certificate::from_pem(pem));
}

