#include <catch2/catch.hpp>
#include "utils.h"

#include "template_tests.h"
#include "x509/X509Store.h"
#include "SslException.h"

TEST_CASE( "x509Store test", "[store][x509]" )
{
    X509Store store;
    REQUIRE_NOTHROW(X509Store());

    SECTION("load default location")
        REQUIRE_NOTHROW(store.loadDefaultLocation());

    SECTION("load dir")
        REQUIRE_NOTHROW(store.loadDirectory("/etc/ssl/certs"));

    SECTION("load file")
    {
        REQUIRE_THROWS_AS(store.loadFile("content/"), SslException);
        REQUIRE_NOTHROW(store.loadDirectory("content/toxchat.crt"));
    }

}

TEST_CASE( "x509Store memory test", "[store][x509]")
{
    X509Store store;
    REQUIRE_NOTHROW(move_test(store));
}
