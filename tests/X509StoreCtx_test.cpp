#include <catch.hpp>
#include <iostream>
#include "utils.h"

#include "template_tests.h"
#include "x509/X509StoreCtx.h"
#include "x509/exceptions/SslVerifyException.h"
#include "SslException.h"

TEST_CASE( "x509StoreCtx memory test", "[storectx][x509]")
{
    X509StoreCtx store;
    REQUIRE_NOTHROW(move_test(store));
}

TEST_CASE( "x509StoreCtx test", "[storectx][x509]")
{
    X509Store store;
    store.loadDefaultLocation();
    X509StoreCtx storeCtx;
    //std::string path = "content/telegramorg.crt";
    std::string path = "content/LetsEncryptAuthorityX3.crt";
    std::string pem = read_file(path);

    X509Certificate cert = X509Certificate::from_pem(pem);

    X509VerifyParam param;
    param.setHost("aaa.com");
    param.setDepth(1);
    storeCtx.setParametrs(std::move(param));
    storeCtx.setStore(std::move(store));
    /*try
    {
        //storeCtx.verify(cert);
    }
    catch (SslVerifyException& ex)
    {
        std::cout << ex.what() << std::endl;
        std::cout << storeCtx.getErrorDepth() << std::endl;
    }*/
    CHECK_THROWS_AS( storeCtx.verify(cert), SslVerifyException);


//    size_t i = 0;
    for(const auto& chainCert : storeCtx.getChain())
    {
        //std::cout << i << " " <<  chainCert.get_subject_name() << std::endl;
        //i ++;
        (void)(chainCert);
    }
}