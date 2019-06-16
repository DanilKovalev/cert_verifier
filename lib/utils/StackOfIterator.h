#pragma once

#include <openssl/stack.h>

#include <memory>

template <typename Type>
class StackOfIterator
{
public:
    typedef Type value_type;
    typedef std::input_iterator_tag iterator_category;
    typedef ptrdiff_t              difference_type;
    typedef Type* pointer;
    typedef Type& reference;
 
    StackOfIterator(const struct stack_st* stack, int position = 0)
    : m_stack(stack)
    , m_type()
    , m_position(position)
    {
        if(position != sk_num(m_stack))
            m_type = std::make_shared<Type>(toRawType(sk_value(m_stack, position)), false);
    }

    StackOfIterator(const StackOfIterator& obj) = default;
    StackOfIterator(StackOfIterator&& obj) noexcept = default;

    StackOfIterator& operator=(const StackOfIterator& obj)
    {
        if (this == &obj)
            return *this;

        StackOfIterator tmp(obj);
        this->swap(tmp);
        return *this;
    }

    StackOfIterator& operator=(StackOfIterator&&) = delete;

    void swap(StackOfIterator& obj)
    {
        std::swap(this->m_type, obj.m_type);
        std::swap(this->m_stack, obj.m_stack);
        std::swap(this->m_position, obj.m_position);
    }
    
    bool operator ==(const StackOfIterator& rhs) const
    { return this->m_type == rhs.m_type;}

    bool operator !=(const StackOfIterator& rhs) const 
    { return this->m_type!= rhs.m_type;}


    StackOfIterator& operator++()
    {
        if(!m_type)
            throw std::invalid_argument("[TODO] cannot advance iterator");
        
        m_position++;
        if(m_position == sk_num(m_stack))
            m_type.reset();
        else
            m_type = std::make_shared<Type>(toRawType(sk_value(m_stack, m_position)), false);

        return *this;
    }

    reference& operator *() const
    {
        return *m_type;
    }

    pointer operator->() const
    {
        return m_type.get();
    }

  private:
    static constexpr typename Type::RawType* toRawType(void* pRaw) noexcept
    {
        return static_cast<typename Type::RawType*>(pRaw);
    };

private:
    const struct stack_st* m_stack;
    std::shared_ptr<Type> m_type;
    int m_position;
};

