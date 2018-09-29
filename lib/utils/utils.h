#pragma once

#include <utility>

#include <openssl/crypto.h>


template <typename T>
class ObjectHolder
{
public:
    explicit ObjectHolder(T* raw) : m_raw(raw) {}

    ObjectHolder(const ObjectHolder&) = delete;
    ObjectHolder(ObjectHolder&& other) noexcept
        : m_raw( std::exchange(other.m_raw, nullptr) )
    {};
    ObjectHolder& operator=(const ObjectHolder& other) = delete;
    ObjectHolder& operator=(ObjectHolder&& other) noexcept
    {
        if (this == &other)
            return *this;

        std::swap(*this, other);
        other.free();
        return *this;
    }

    void swap(ObjectHolder& other) noexcept
    { std::swap(*this, other); }

    ~ObjectHolder() {free();}

    T* raw() { return m_raw;};
    const T* raw() const { return m_raw;};

private:
    void free() noexcept
    {
        OPENSSL_free(m_raw);
    }

private:
    T* m_raw;
};

template <typename T>
ObjectHolder<T> makeObjectHolder(T* obj)
{
    return ObjectHolder<T>(obj);
}

namespace std
{
    template <typename T>
    inline void swap(ObjectHolder<T>& a, ObjectHolder<T>& b) noexcept
    {
        a.swap(b);
    }
}


