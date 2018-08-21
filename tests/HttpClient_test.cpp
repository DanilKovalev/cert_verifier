#include <catch.hpp>
#include <iostream>
#include "utils.h"

#include "template_tests.h"
#include "http/HttpClient.h"

TEST_CASE( "HttpClient memory test", "[http][net]")
{
    HttpClient client("google.comn");
    REQUIRE_NOTHROW(move_test(client));
}

TEST_CASE( "HttpClient test", "[http]")
{
    HttpClient client("google.com");
    client.followLocation(true);

    CHECK_NOTHROW(client.perform());
}