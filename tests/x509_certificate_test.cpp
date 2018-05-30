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

TEST_CASE("Cert memory test", "[store][x509]")
{
    std::string path = "content/cert.pem";
    std::string pem = read_file(path);

    x509_certificate cert1 = x509_certificate::from_pem(pem);
    x509_certificate cert2(cert1);

    std::swap(cert2, cert1);
    cert1 = std::move(cert2);
    x509_certificate cert3(std::move(cert1));
}
