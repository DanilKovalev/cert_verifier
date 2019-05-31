#pragma once
#include "type_traits"

#include <iostream>

#include <catch2/catch.hpp>
#include <utils/ObjectHelper.h>

template <typename T>
void move_test(T& obj)
{
    T copy(std::move(obj));
    obj = std::move(copy);
};

template<class T>
struct canDuplicate
{
    template<typename C>
    static constexpr decltype(T::duplicate, bool()) test(int /* unused */)
    {
        return true;
    }

    template<typename C>
    static constexpr bool test(...)
    {
        return false;
    }

    static constexpr bool value = test<T>(int());
};

template<class T>
typename std::enable_if<!canDuplicate<T>::value>::type copy_test(T& obj)
{
    T copy(obj);
    CHECK_NOTHROW(obj = copy);
}

template<class T>
typename std::enable_if<canDuplicate<T>::value>::type copy_test(T& obj)
{
    T copy(obj);
    CHECK_NOTHROW(obj = copy);
    copy = ObjectHelper<T>::makeCopied(obj.raw());
    obj = ObjectHelper<T>::makeAttached(copy.detach());
}

template<typename T>
void memory_tests(T& obj)
{
    REQUIRE_NOTHROW(move_test(obj));
    REQUIRE_NOTHROW(copy_test(obj));
}
