#include "X509ExtensionIterator.h"

X509ExtensionIterator::X509ExtensionIterator()
 : m_extList(nullptr)
 , m_num(0)
 , m_size(0)
 , m_ext()
{}


X509ExtensionIterator::X509ExtensionIterator(X509_EXTENSIONS* exts, int position)
 : m_extList(exts)
 , m_num(position)
 , m_size(sk_X509_EXTENSION_num(m_extList))
 , m_ext(create_extension_ptr())
{

}

const X509Extension& X509ExtensionIterator::operator*() const
{
    return *m_ext;
}

X509ExtensionIterator &X509ExtensionIterator::operator++()
{
    if(++m_num < m_size)
    {
        m_ext = create_extension_ptr();
    }
    else
    {
        m_ext.reset();
        m_extList = nullptr;
        m_size = m_num = 0;
    }

    return *this;
}

bool X509ExtensionIterator::operator==(const X509ExtensionIterator &rhs) const noexcept
{
    return m_extList == rhs.m_extList && m_num == rhs.m_num;
}

bool X509ExtensionIterator::operator!=(const X509ExtensionIterator &rhs) const noexcept
{
    return !(*this == rhs);
}

std::shared_ptr<X509Extension> X509ExtensionIterator::create_extension_ptr()
{
    X509_EXTENSION* ext = sk_X509_EXTENSION_value(m_extList, m_num);
    return std::make_shared<X509Extension>( X509Extension::create_wrapper(ext));
}

