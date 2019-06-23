#pragma once

#include "X509Extension.h"
#include "utils/StackOf.h"
#include <optional>

class X509ExtensionsStack : public StackOf<X509Extension>
{
  public:
    X509ExtensionsStack(X509_EXTENSIONS* exts, bool acquire) noexcept
      : StackOf(reinterpret_cast<struct stack_st*>(exts), acquire)
    {
    }

    explicit X509ExtensionsStack(StackOf&& stack) noexcept
      : StackOf(stack)
    {
    }

    explicit X509ExtensionsStack(StackOf& stack)
      : StackOf(stack)
    {
    }

    X509ExtensionsStack(const X509ExtensionsStack& other) = default;
    X509ExtensionsStack(X509ExtensionsStack&& other) = default;

    X509ExtensionsStack& operator=(const X509ExtensionsStack& other)
    {
        StackOf::operator=(other);
        return *this;
    }

    X509ExtensionsStack& operator=(X509ExtensionsStack&& other) noexcept
    {
        StackOf::operator=(std::move(other));
        return *this;
    }

    template <typename ExtType>
    std::optional<ExtType> findExtension() const noexcept;

    template <typename ExtType>
    ExtType getExtension() const;

    ~X509ExtensionsStack() = default;
};

#include "X509ExtensionsStack_impl.h"