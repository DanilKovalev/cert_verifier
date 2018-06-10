#include "X509ExtensionList.h"
#include <boost/numeric/conversion/cast.hpp>

X509ExtensionList::X509ExtensionList(X509_EXTENSIONS *ext_list, bool acquire)
: m_extList(ext_list)
, m_acquired(acquire)
{
}

size_t X509ExtensionList::size() const noexcept
{
    return boost::numeric_cast<size_t>(sk_X509_EXTENSION_num(m_extList));
}

X509Extension X509ExtensionList::operator[](size_t num) const
{
    X509_EXTENSION* ext = sk_X509_EXTENSION_value(m_extList, boost::numeric_cast<int>(num));
    return X509Extension::create_wrapper(ext);
}

X509ExtensionIterator X509ExtensionList::begin() noexcept
{
    return X509ExtensionIterator(m_extList, 0);
}

X509ExtensionIterator X509ExtensionList::end() noexcept
{
    return X509ExtensionIterator();
}
