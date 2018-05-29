#include <catch.hpp>
#include "utils.h"

#include "X509StoreCtx.h"
#include "SslException.h"

TEST_CASE( "x509StoreCtx memory test", "[storectx][x509]")
{
    X509StoreCtx store;
    X509StoreCtx store2;

    std::swap(store, store2);
    store = std::move(store2);
    X509StoreCtx store3(std::move(store));
}
