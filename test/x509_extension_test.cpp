#include <catch.hpp>
#include <fstream>
#include <iostream>

#include "x509_certificate.h"
#include "x509_extension_iterator.h"
#include "x509_extension_list.h"
#include "extensions/authority_information_access.h"
#include "extensions/crl_distribution_points.h"
#include <openssl/x509v3.h>

TEST_CASE( "extension print", "[cert][extension]" )
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

    std::ifstream file(path);
    if (!file.is_open())
        std::__throw_system_error(errno);

    std::string pem((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
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
        auto it = std::find_if(extensions.begin(), extensions.end(),
                               [](const x509_extension& ext) -> bool{
                                   return ext.nid() == NID_crl_distribution_points;
                               });

        REQUIRE(it != extensions.end());

        x509_extension extension = *it;
        crl_distribution_points point(extension);


    }
}