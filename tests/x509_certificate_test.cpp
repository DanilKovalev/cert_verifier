#include <catch.hpp>
#include "utils.h"

#include "x509/X509Certificate.h"

TEST_CASE( "Cert read", "[cert]" )
{
    std::string path = "content/";
    SECTION("cert")
        path += "cert.pem";

    SECTION("telegramorg")
        path += "telegramorg.crt";

    SECTION("toxchat.crt")
        path += "toxchat.crt";

    SECTION("letsEncrypt.crt")
        path += "LetsEncryptAuthorityX3.crt";

    std::string pem = read_file(path);
    REQUIRE_NOTHROW(X509Certificate::from_pem(pem));
}

TEST_CASE("Cert memory test", "[store][x509]")
{
    std::string path = "content/cert.pem";
    std::string pem = read_file(path);

    X509Certificate cert1 = X509Certificate::from_pem(pem);
    X509Certificate cert2(cert1);

    std::swap(cert2, cert1);
    cert1 = std::move(cert2);
    X509Certificate cert3(std::move(cert1));
}
