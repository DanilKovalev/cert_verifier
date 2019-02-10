#include <catch2/catch.hpp>
#include "utils.h"
#include "utils/StackOf.h"

#include "x509/X509Certificate.h"
#include "x509/extensions/AuthorityInformationAccess.h"
#include "x509/extensions/CrlDistributionPoints.h"
#include <openssl/x509v3.h>
#include <iostream>
#include <unordered_map>

TEST_CASE( "extension print", "[cert][extension]" )
{
    std::unordered_map<std::string, std::string> content
    {
            {"simple pem", "cert.pem"},
            {"telegramorg", "telegramorg.crt"},
            {"toxchat.crt", "toxchat.crt"}
    };


    for(const auto& kv : content)
    {
        DYNAMIC_SECTION("certificate name: " << kv.first)
        {
            std::string pem = read_file("content/" + kv.second);
            X509Certificate certificate = X509Certificate::from_pem(pem);
            StackOf<X509Extension> extensions = certificate.get_extensions();

            for(const auto& extension : extensions)
                (void) extension;
        }
    }
}

TEST_CASE( "AuthorityInformationAccess", "[cert][extension]" )
{
    
    std::string path = "content/telegramorg.crt";
    std::string oscp = "http://ocsp.godaddy.com/";
    std::string issuer = "http://certificates.godaddy.com/repository/gdig2.crt";

    std::string pem = read_file(path);
    X509Certificate certificate = X509Certificate::from_pem(pem);

    StackOf<X509Extension> extensions = certificate.get_extensions();
    auto it = std::find_if(extensions.cbegin(), extensions.cend(),
                 [](const X509Extension& ext) -> bool{
                     return ext.nid() == NID_info_access;
                 });

    REQUIRE(it != extensions.cend());

    X509Extension extension = *it;
    AuthorityInformationAccess auth(extension);

    REQUIRE(auth.oscp() == oscp);
    REQUIRE(auth.ca_issuer() == issuer);
}

TEST_CASE( "CrlDistributionPoints", "[cert][extension][crl]" )
{
    std::string path = "content/telegramorg.crt";
    std::string pem = read_file(path);

    X509Certificate certificate = X509Certificate::from_pem(pem);
    StackOf<X509Extension> extensions = certificate.get_extensions();

    REQUIRE(certificate.hasExtensions());
    auto it = std::find_if(extensions.begin(), extensions.end(),
                           [](const X509Extension& ext) -> bool{
                               return ext.nid() == NID_crl_distribution_points;
                           });

    REQUIRE(it != extensions.end());
    X509Extension extension = *it;
    CrlDistributionPoints points = CrlDistributionPoints::fromExtension(extension);

    REQUIRE(points.size() == 1);

    DistPoint point = points[0];
    REQUIRE(point.get_crl_issuers().empty());
    REQUIRE(point.get_distribution_point_names().size() == 1);
}
