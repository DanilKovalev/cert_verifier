#include "x509_extension_iterator.h"

x509_extension_iterator::x509_extension_iterator()
 : m_ext_list(nullptr)
 , m_num(0)
 , m_size(0)
 , m_ext()
{}


x509_extension_iterator::x509_extension_iterator(X509_EXTENSIONS* exts, int position)
 : m_ext_list(exts)
 , m_num(position)
 , m_size(sk_X509_EXTENSION_num(m_ext_list))
 , m_ext(create_extension_ptr())
{

}

const x509_extension& x509_extension_iterator::operator*() const
{
    return *m_ext;
}

x509_extension_iterator &x509_extension_iterator::operator++()
{
    if(++m_num < m_size)
    {
        m_ext = create_extension_ptr();
    }
    else
    {
        m_ext.reset();
        m_ext_list = nullptr;
        m_size = m_num = 0;
    }

    return *this;
}

bool x509_extension_iterator::operator==(const x509_extension_iterator &rhs) const noexcept
{
    return m_ext_list == rhs.m_ext_list && m_num == rhs.m_num;
}

bool x509_extension_iterator::operator!=(const x509_extension_iterator &rhs) const noexcept
{
    return !(*this == rhs);
}

std::shared_ptr<x509_extension> x509_extension_iterator::create_extension_ptr()
{
    X509_EXTENSION* ext = sk_X509_EXTENSION_value(m_ext_list, m_num);
    return std::make_shared<x509_extension>( x509_extension::create_wrapper(ext));
}

