#pragma once

#include <openssl/safestack.h>

class Stack
{
public:
    Stack();
    Stack(const Stack& other);
    Stack(Stack&& other) noexcept;

    Stack& operator= (const Stack& rhs);
    Stack& operator= (Stack&& rhs) noexcept;

    ~Stack();

    void swap(Stack& other) noexcept;
    void free() noexcept;
    
    void* operator[](int) const;
    int push(void*);
    
    int size() const;

private:
    static stack_st* duplicate(stack_st* stack);
    
private:
    stack_st* m_stack;
    bool m_acquired;
};

namespace std
{
    inline void swap(Stack& a, Stack& b) noexcept
    {
        a.swap(b);
    }
}


