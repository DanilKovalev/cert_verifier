#pragma once

#include "SslException.h"
#include "StackOfIterator.h"

#include <openssl/stack.h>
#include <cstdio>

template <typename Type>
class StackOf
{
public:
    typedef StackOf<Type> iterator;
    typedef StackOf<const Type> const_iterator;
    typedef Type value_type;
    typedef Type& reference;
    typedef const Type& const_reference;
    typedef Type* pointer;
    typedef const Type * const_pointer;

public:
    StackOf() : m_stack(createStack()) , m_acquired(true)
    {};

    explicit StackOf(const struct stack_st* stack) : m_stack(duplicate(stack)), m_acquired(true){};

    StackOf(struct stack_st* stack, bool acquire) : m_stack(stack), m_acquired(acquire){};

    StackOf(const StackOf& obj)
    : m_stack(duplicate(obj.m_stack))
    , m_acquired(true)
    {};

    StackOf(StackOf&& obj) noexcept
    : m_stack(std::exchange(obj.m_stack, nullptr))
    , m_acquired(std::exchange(obj.m_acquired, false))
    {};

    StackOf& operator =(const StackOf& obj)
    {
        if (this == &obj)
            return *this;

        StackOf tmp(obj);
        this->swap(tmp);
        return *this;    
    }

    StackOf& operator =(StackOf&& obj) noexcept
    {
        if(this == &obj)
            return *this;

        this->swap(obj);
        obj.clear();
        return *this;
    }
    
    virtual ~StackOf()
    {
        if (!m_acquired || m_stack == nullptr)
            return;

        clear();
        sk_free(m_stack);
    };

    struct stack_st* detach() noexcept
    {
        m_acquired = false;
        return std::exchange(m_stack, nullptr);
    }

    struct stack_st* raw() noexcept
    {
        return m_stack;
    }

    const struct stack_st* raw() const noexcept
    {
        return m_stack;
    }

    const Type operator[](int i) const
    {
        return Type(toRawType(sk_value(m_stack, i)), false);
    }

    Type operator[](int i)
    {
        return Type(toRawType(sk_value(m_stack, i)), false);
    }

    const Type front() const
    {
        return Type(toRawType(sk_value(m_stack, 0)), false);
    }

    const Type back() const
    {
        return Type(toRawType(sk_value(m_stack, size() - 1)), false);
    }

    void push(const Type& value)
    {
        Type newValue(value);
        pushImpl(newValue.detach());
    }

    void push(Type&& value)
    {
        pushImpl(value.detach());
    }

    int size() const
    {
        return sk_num(m_stack);
    }

    void swap(StackOf& other) noexcept
    {
        std::swap(this->m_stack, other.m_stack);
        std::swap(this->m_acquired, other.m_acquired);
    }

    void clear()
    {
        if(m_acquired)
            clearImpl();

        sk_zero(m_stack);
    }

    StackOfIterator<Type> begin() noexcept
    {
        return StackOfIterator<Type>(m_stack);
    }

    StackOfIterator<Type> end() noexcept
    {
        return StackOfIterator<Type>(m_stack, size());
    }
    
    StackOfIterator<const Type> cbegin() noexcept
    {
        return StackOfIterator<const Type>(m_stack);
    }

    StackOfIterator<const Type> cend() noexcept
    {
        return StackOfIterator<const Type>(m_stack, size());
    }

    struct stack_st* duplicate(const struct stack_st* stack)
    {
        StackOf newStack;
        for (int i = 0; i < sk_num(stack); ++i)
        {
            Type value(Type::duplicate(toRawType(sk_value(stack, i))), true);
            newStack.push(std::move(value));
        }

        return newStack.detach();
    }
private:

    void clearImpl()
    {
        void* pRaw = nullptr;
        while ((pRaw = sk_pop(m_stack)) != nullptr)
            Type type(toRawType(pRaw), true);
    }

    static constexpr typename Type::RawType* toRawType(void* pRaw) noexcept
    { 
        return static_cast<typename Type::RawType*>(pRaw);
    };

    static struct stack_st* createStack()
    {
        struct stack_st* result = sk_new_null();
        if (result)
            return result;

        throw SslException("sk_new_null");
    }

    void pushImpl(typename Type::RawType* pRaw)
    {
        if(sk_push(m_stack, pRaw) == 0)
            throw SslException("sk_push");
    }

private:
    struct stack_st* m_stack;
    bool m_acquired;
};

namespace std
{
    template <typename T>
    inline void swap(StackOf<T>& a, StackOf<T>& b) noexcept
    {
        a.swap(b);
    }
}

