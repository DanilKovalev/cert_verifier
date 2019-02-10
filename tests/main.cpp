#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>

#include "Instance.h"

int main(int argc, char* const argv[])
{
    auto& ssl_instance = Instance::get();
    (void)ssl_instance;
    int result = Catch::Session().run(argc, argv);

    return result;
}