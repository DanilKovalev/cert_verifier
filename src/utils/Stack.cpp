#include "SslException.h"
#include "Stack.h"
#include <boost/numeric/conversion/cast.hpp>

Stack::Stack()
: m_stack(sk_new_null())
, m_acquired(true)
{
    if (!m_stack)
        throw SslException("Failed to create stack");
}

Stack::Stack(const Stack& other)
: m_stack(duplicate(other.m_stack))
, m_acquired(true)
{

}

Stack::Stack(Stack&& other) noexcept
: m_stack(std::exchange(other.m_stack, nullptr))
, m_acquired(std::exchange(other.m_acquired, false))
{
}

Stack& Stack::operator=(const Stack& other)
{
    if (this == &other)
        return *this;

    Stack tmp(other);
    swap(tmp);
    tmp.free();
    
    return *this;;
}

Stack& Stack::operator=(Stack&& other) noexcept
{
    if (this == &other)
        return *this;

    swap(other);
    other.free();
    return *this;
}

Stack::~Stack()
{
    free();
}


int Stack::size() const
{
    return sk_num(m_stack);
}

stack_st* Stack::duplicate(stack_st* stack)
{
    stack_st *result = sk_dup(stack);
    if(result == nullptr)
        throw SslException("Failed to duplicate stack");

    return result;
}

void* Stack::operator[](int indx) const
{
    return sk_value(m_stack, boost::numeric_cast<int>(indx));
}

int Stack::push(void* data)
{
    return sk_push(m_stack, data);
}

void Stack::swap(Stack& other) noexcept
{
    std::swap(other.m_stack, this->m_stack);
    std::swap(other.m_acquired, this->m_acquired);
}

void Stack::free() noexcept 
{
    if(m_stack && m_acquired)
        sk_free(m_stack);

    m_stack = nullptr;
    m_acquired = false;
}

