#pragma once

#include "x509_extension.h"

#include <memory>
#include <system_error>


class x509_extension_iterator
{
public:
    typedef x509_extension value_type;
    typedef std::input_iterator_tag iterator_category;
    typedef ptrdiff_t              difference_type;
    typedef const x509_extension* pointer;
    typedef const x509_extension& reference;

    x509_extension_iterator();
    explicit x509_extension_iterator(X509_EXTENSIONS* exts, int position = 0);

    x509_extension_iterator(const x509_extension_iterator&) = default;
    x509_extension_iterator(x509_extension_iterator&&) = default;

    x509_extension_iterator& operator=(const x509_extension_iterator&) = default;
    x509_extension_iterator& operator=(x509_extension_iterator&&) = default;

    ~x509_extension_iterator() = default;

    reference& operator *() const;

    x509_extension_iterator& operator++();

    bool operator == (const x509_extension_iterator& rhs) const noexcept;
    bool operator != (const x509_extension_iterator& rhs) const noexcept;

private:
    std::shared_ptr<x509_extension> create_extension_ptr();

private:
    STACK_OF(X509_EXTENSION)* m_ext_list;
    int m_num;
    int m_size;
    std::shared_ptr<x509_extension> m_ext;
};

