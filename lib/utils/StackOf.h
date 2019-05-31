#pragma once

#include "ObjectHolder.h"
#include "SslException.h"
#include "StackOfIterator.h"

#include <openssl/stack.h>

template<typename Type>
class StackOf : public ObjectHolder<stack_st, StackOf<Type>>
{
  public:
    typedef StackOf<Type> iterator;
    typedef StackOf<const Type> const_iterator;
    typedef Type value_type;
    typedef Type& reference;
    typedef const Type& const_reference;
    typedef Type* pointer;
    typedef const Type* const_pointer;
    typedef stack_st RawType;

  public:
    StackOf()
      : ObjectHolder<struct stack_st, StackOf<Type>>(createStack(), true){};

    explicit StackOf(const struct stack_st* raw)
      : ObjectHolder<struct stack_st, StackOf<Type>>(duplicate(raw), true){};

    StackOf(struct stack_st* raw, bool acquire)
      : ObjectHolder<struct stack_st, StackOf<Type>>(raw, acquire){};

    StackOf(const StackOf& obj) = default;

    StackOf(StackOf&& obj) = default;

    StackOf& operator=(const StackOf& obj)
    {
        ObjectHolder<struct stack_st, StackOf<Type>>::operator=(obj);
        return *this;
    }

    StackOf& operator=(StackOf&& obj) noexcept
    {
        ObjectHolder<struct stack_st, StackOf<Type>>::operator=(std::move(obj));
        return *this;
    }

    ///@todo: make not virtual
    virtual ~StackOf() = default;

    const Type operator[](int i) const
    {
        return Type(toRawType(sk_value(this->m_raw, i)), false);
    }

    Type operator[](int i)
    {
        return Type(toRawType(sk_value(this->m_raw, i)), false);
    }

    const Type front() const
    {
        return Type(toRawType(sk_value(this->m_raw, 0)), false);
    }

    const Type back() const
    {
        return Type(toRawType(sk_value(this->m_raw, size() - 1)), false);
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
        return sk_num(this->m_raw);
    }

    StackOfIterator<Type> begin() noexcept
    {
        return StackOfIterator<Type>(this->m_raw);
    }

    StackOfIterator<Type> end() noexcept
    {
        return StackOfIterator<Type>(this->m_raw, size());
    }

    StackOfIterator<const Type> cbegin() noexcept
    {
        return StackOfIterator<const Type>(this->m_raw);
    }

    StackOfIterator<const Type> cend() noexcept
    {
        return StackOfIterator<const Type>(this->m_raw, size());
    }

    static struct stack_st* duplicate(const struct stack_st* stack)
    {
        StackOf newStack;
        for (int i = 0; i < sk_num(stack); ++i)
        {
            Type value(Type::duplicate(toRawType(sk_value(stack, i))), true);
            newStack.push(std::move(value));
        }

        return newStack.detach();
    }

    static void destroy(struct stack_st* raw)
    {
        void* pItem = nullptr;
        while ((pItem = sk_pop(raw)) != nullptr)
            Type::destroy(toRawType(pItem));

        sk_zero(raw);
        sk_free(raw);
    }

  private:
    static constexpr typename Type::RawType* toRawType(void* pRaw) noexcept
    {
        return static_cast<typename Type::RawType*>(pRaw);
    };

    static struct stack_st* createStack()
    {
        struct stack_st* result = sk_new_null();
        if (!result)
            throw SslException("sk_new_null");

        return result;
    }

    void pushImpl(typename Type::RawType* pRaw)
    {
        if (sk_push(this->m_raw, pRaw) == 0)
            throw SslException("sk_push");
    }
};
