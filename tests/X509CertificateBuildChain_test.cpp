#include <catch.hpp>
#include "utils.h"
#include "template_tests.h"

#include "x509/X509Certificate.h"
#include "x509/X509StoreCtx.h"
#include "x509/extensions/AuthorityInformationAccess.h"
#include "http/HttpClient.h"

#include <iostream>

TEST_CASE( "Build chain test", "[cert][connect]" )
{
    StackOf<X509Certificate> additionalCertificates;
    std::string path = "content/";
    std::string hostName;
    SECTION("telegram")
    {
        //additionalCertificates.push(read_cert(path + "GoDaddySecureCertificateAuthority-G2.crt"));

        path += "telegramorg.crt";
        hostName = "telegram.org";
    }

    X509Certificate certificate = read_cert(path);

    X509Store store;
    store.loadDefaultLocation();
    X509StoreCtx storeCtx;
    storeCtx.setStore(std::move(store));

    X509VerifyParam param;
//    param.setHost(hostName);
    param.setDepth(2);
    storeCtx.setParametrs(std::move(param));


    SslVerifyException exception;
//    REQUIRE_FALSE(storeCtx.verify(certificate, exception));
//    REQUIRE(exception.getCode() == X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT_LOCALLY);

    REQUIRE(certificate.hasExtensions());
    StackOf<X509Extension> extensions = certificate.get_extensions();
    auto it = std::find_if(extensions.begin(), extensions.end(),
                           [](const X509Extension& ext) -> bool {
                               return ext.nid() == NID_info_access;
                           });

    REQUIRE(it != extensions.end());
    X509Extension extension = *it;
    AuthorityInformationAccess authorityInformationAccess(extension);

    std::cout << authorityInformationAccess.oscp() << std::endl;
    std::cout << authorityInformationAccess.ca_issuer() << std::endl;
//    std::cout << "body: \n"<< download(authorityInformationAccess.ca_issuer()) << std::endl;

    X509Certificate additionalCert = X509Certificate::from_der(HttpClient::request(authorityInformationAccess.ca_issuer()));
    additionalCertificates.push(additionalCert);
//    get_http(authorityInformationAccess.ca_issuer());

//    X509Store store2;
//    store2.loadDefaultLocation();
//    storeCtx.setStore(std::move(store2));
//    storeCtx.setParametrs(X509VerifyParam());
    storeCtx.setAdditionalCertificates(additionalCertificates);
    CHECK(storeCtx.verify(certificate, exception));

    std::cout << exception.what() << std::endl;
    std::cout << exception.getCode() << std::endl;
    std::cout << storeCtx.getErrorDepth() << std::endl;
}
