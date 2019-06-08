#include "template_tests.h"
#include "utils.h"
#include "x509/X509Crl.h"

#include <unordered_map>

#include <catch2/catch.hpp>

TEST_CASE("CRL read der", "[crl]")
{
    std::unordered_map<std::string, std::string> content{ { "amazon crl der", "sca1b.crl" } };

    for (const auto& tc : content)
    {
        DYNAMIC_SECTION("crl name: " << tc.first)
        {
            std::vector<uint8_t> der = read_binary_file("content/crls/" + tc.second);
            X509Crl crl = X509Crl::fromDer(der);
            REQUIRE_NOTHROW(X509Crl::fromPem(crl.toPem()));
        }
    }
}

TEST_CASE("CRL extensions test", "[crl]")
{
    std::vector<uint8_t> der = read_binary_file("content/crls/sca1b.crl");
    X509Crl crl = X509Crl::fromDer(der);
    for (const X509Extension& ext : crl.getExtensions())
    {
        (void)ext;
    }
}
