#include "utils/Stack.h"

#include <catch.hpp>
#include <openssl/err.h>
#include <iostream>


TEST_CASE( "stack test ", "[stack]" )
{
    char *p;
    Stack stack;
}

TEST_CASE("Stack memory test", "[stack][memory]")
{
    char *p;
    Stack stack1;
    stack1.push(p);
    
    Stack stack2(stack1);

    std::swap(stack1, stack1);
    stack1 = std::move(stack2);
    Stack stack3(std::move(stack1));
}


