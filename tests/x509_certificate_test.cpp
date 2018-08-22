#include <catch.hpp>
#include "utils.h"

#include "template_tests.h"
#include "x509/X509Certificate.h"

TEST_CASE( "Cert read ", "[cert]" )
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
    X509Certificate cert = X509Certificate::from_pem(pem);
    REQUIRE_NOTHROW(X509Certificate::from_der(cert.to_der()));
}

TEST_CASE("Cert memory test", "[store][x509]")
{
    std::string path = "content/cert.pem";
    std::string pem = read_file(path);

    X509Certificate cert = X509Certificate::from_pem(pem);
    REQUIRE_NOTHROW(copy_test(cert));
    REQUIRE_NOTHROW(move_test(cert));
}
