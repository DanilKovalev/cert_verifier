#pragma once

#include "X509Extension.h"

#include <memory>
#include <system_error>


class X509ExtensionIterator
{
public:
    typedef X509Extension value_type;
    typedef std::input_iterator_tag iterator_category;
    typedef ptrdiff_t              difference_type;
    typedef const X509Extension* pointer;
    typedef const X509Extension& reference;

    X509ExtensionIterator();
    explicit X509ExtensionIterator(X509_EXTENSIONS* exts, int position = 0);

    X509ExtensionIterator(const X509ExtensionIterator&) = default;
    X509ExtensionIterator(X509ExtensionIterator&&) = default;

    X509ExtensionIterator& operator=(const X509ExtensionIterator&) = default;
    X509ExtensionIterator& operator=(X509ExtensionIterator&&) = default;

    ~X509ExtensionIterator() = default;

    reference& operator *() const;

    X509ExtensionIterator& operator++();

    bool operator == (const X509ExtensionIterator& rhs) const noexcept;
    bool operator != (const X509ExtensionIterator& rhs) const noexcept;

private:
    std::shared_ptr<X509Extension> create_extension_ptr();

private:
    STACK_OF(X509_EXTENSION)* m_extList;
    int m_num;
    int m_size;
    std::shared_ptr<X509Extension> m_ext;
};

