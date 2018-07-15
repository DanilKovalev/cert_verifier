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
    
public:
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
    
    ~StackOf()
    {
        if (!m_acquired)
            return;

        clear();
        sk_free(m_stack);
    };
    
    const Type operator[](int i) const
    {
        return Type(toRawType(sk_value(m_stack, i)), false);
    }

    Type operator[](int i)
    {
        return Type(toRawType(sk_value(m_stack, i)), false);
    }

    void push(Type& type)
    {
        if(m_acquired)
            sk_push(m_stack, type.detach());
        else
            sk_push(m_stack, type.raw());
    }

    void push(Type&& type)
    {
        sk_push(m_stack, type.detach());
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
    
private:
    struct stack_st* duplicate(struct stack_st* stack)
    {
        stack_st *result = sk_dup(stack);
        if(result == nullptr)
            throw SslException("[TODO] Failed to duplicate stack");

        return result;
    } 

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
    
    
private:
    struct stack_st* m_stack;
    bool m_acquired;
};


