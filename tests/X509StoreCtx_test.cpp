#include <catch.hpp>
#include "utils.h"

#include "x509/X509StoreCtx.h"
#include "SslException.h"

TEST_CASE( "x509StoreCtx memory test", "[storectx][x509]")
{
    X509StoreCtx store;
    X509StoreCtx store2;

    std::swap(store, store2);
    store = std::move(store2);
    X509StoreCtx store3(std::move(store));
}


TEST_CASE( "x509StoreCtx test", "[storectx][x509]")
{
    X509Store store;
    store.loadDefaultLocation();
    X509StoreCtx storeCtx;
    std::string path = "content/cert.pem";
    std::string pem = read_file(path);

    X509Certificate cert = X509Certificate::from_pem(pem);

    storeCtx.setStore(std::move(store));
    CHECK_THROWS( storeCtx.verify(cert));
}