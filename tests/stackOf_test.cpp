#include <catch.hpp>
#include "utils.h"

#include "template_tests.h"
#include "utils/StackOf.h"
#include "x509/X509Certificate.h"

TEST_CASE("Stack memory test", "[stackOf]")
{
    std::string path = "content/cert.pem";
    X509Certificate cert1 = X509Certificate::from_pem(read_file("content/cert.pem"));
    X509Certificate cert2 = X509Certificate::from_pem(read_file("content/toxchat.crt"));

    StackOf<X509Certificate> stack;
    stack.push(cert1);
    stack.push(std::move(cert2));

    REQUIRE_NOTHROW(copy_test(stack));
    REQUIRE_NOTHROW(move_test(stack));
}
