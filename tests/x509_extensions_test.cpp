#include <catch.hpp>
#include "utils.h"

#include "x509_certificate.h"
#include "x509_extension_iterator.h"
#include "x509_extension_list.h"
#include "extensions/authority_information_access.h"
#include "extensions/crl_distribution_points.h"
#include <openssl/x509v3.h>
#include <iostream>

TEST_CASE( "extension print", "[cert][extension]" )
{
    std::string path = "content/";
    SECTION("cert")
        path += "cert.pem";

    SECTION("telegramorg")
        path += "telegramorg.crt";

    SECTION("toxchat.crt")
        path += "toxchat.crt";

    std::string pem = read_file(path);
    x509_certificate certificate = x509_certificate::from_pem(pem);

    x509_extension_list extensions = certificate.get_extensions();
    for (const auto &p: extensions)
    {
    }


}

TEST_CASE( "authority_information_access", "[cert][extension]" )
{
    std::string path = "content/telegramorg.crt";
    std::string oscp = "http://ocsp.godaddy.com/";
    std::string issuer = "http://certificates.godaddy.com/repository/gdig2.crt";

    std::string pem = read_file(path);
    x509_certificate certificate = x509_certificate::from_pem(pem);

    x509_extension_list extensions = certificate.get_extensions();
    auto it = std::find_if(extensions.begin(), extensions.end(),
                 [](const x509_extension& ext) -> bool{
                     return ext.nid() == NID_info_access;
                 });

    REQUIRE(it != extensions.end());

    x509_extension extension = *it;
    authority_information_access auth(extension);

    REQUIRE(auth.oscp() == oscp);
    REQUIRE(auth.ca_issuer() == issuer);


    {

    }
}

TEST_CASE( "crl_distribution_points", "[cert][extension][crl]" )
{
    std::string path = "content/telegramorg.crt";
    std::string pem = read_file(path);

    x509_certificate certificate = x509_certificate::from_pem(pem);
    x509_extension_list extensions = certificate.get_extensions();

    auto it = std::find_if(extensions.begin(), extensions.end(),
                           [](const x509_extension& ext) -> bool{
                               return ext.nid() == NID_crl_distribution_points;
                           });

    REQUIRE(it != extensions.end());

    x509_extension extension = *it;
    crl_distribution_points points(extension);

    REQUIRE(points.size() == 1);

    dist_point point = points[0];
    REQUIRE(point.get_crl_issuers().empty());
    REQUIRE(point.get_distribution_point_names().size() == 1);
    REQUIRE(point.get_distribution_point_names().back() == "http://crl.godaddy.com/gdig2s1-111.crl");
}
