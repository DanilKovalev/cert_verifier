#include <catch2/catch.hpp>
#include <unordered_map>
#include "utils.h"

#include "template_tests.h"
#include "x509/X509Certificate.h"

TEST_CASE( "Cert read ", "[cert]" )
{
    std::unordered_map<std::string, std::string> content
            {
                    {"simple pem", "cert.pem"},
                    {"telegramorg", "telegramorg.crt"},
                    {"toxchat.crt", "toxchat.crt"},
                    {"letsEncrypt.crt", "LetsEncryptAuthorityX3.crt"}
            };

    for (const auto& kv : content )
    {
        DYNAMIC_SECTION("cert name: " << kv.first)
        {
            std::string pem = read_file("content/" + kv.second);
            REQUIRE_NOTHROW(X509Certificate::from_pem(pem));
            X509Certificate cert = X509Certificate::from_pem(pem);
            REQUIRE_NOTHROW(X509Certificate::from_der(cert.to_der()));
        }
    }
}

TEST_CASE("Cert memory test", "[store][x509]")
{
    std::string path = "content/cert.pem";
    std::string pem = read_file(path);
    X509Certificate cert = X509Certificate::from_pem(pem);

    memory_tests(cert);
}
