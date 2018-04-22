#include "x509_extension_list.h"
#include <boost/numeric/conversion/cast.hpp>

x509_extension_list::x509_extension_list(X509_EXTENSIONS *ext_list, bool acquire)
: m_ext_list(ext_list)
, m_acquired(acquire)
{
}

size_t x509_extension_list::size() const noexcept
{
    return boost::numeric_cast<size_t>(sk_X509_EXTENSION_num(m_ext_list));
}

x509_extension x509_extension_list::operator[](size_t num) const
{
    X509_EXTENSION* ext = sk_X509_EXTENSION_value(m_ext_list, boost::numeric_cast<int>(num));
    return x509_extension::create_wrapper(ext);
}

x509_extension_iterator x509_extension_list::begin() noexcept
{
    return x509_extension_iterator(m_ext_list, 0);
}

x509_extension_iterator x509_extension_list::end() noexcept
{
    return x509_extension_iterator();
}
