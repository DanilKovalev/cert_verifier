#pragma once
#include <catch.hpp>

template <typename T>
void move_test(T& type1)
{
    T type2(std::move(type1));
    type1 = std::move(type2);
};

template <typename T>
void copy_test(T& type1)
{
    T type2(type1);
    type1 = type2;
};




