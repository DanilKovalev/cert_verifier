#define CATCH_CONFIG_RUNNER
#include <catch.hpp>

#include "instance.h"

int main(int argc, char* const argv[])
{
    auto& ssl_instance = instance::get();
    (void)ssl_instance;
    int result = Catch::Session().run(argc, argv);

    return result;
}