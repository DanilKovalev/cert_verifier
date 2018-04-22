#pragma once

#include <memory>
#include "x509_extension.h"
#include "x509_extension_iterator.h"

class x509_extension_list
{
public:
    x509_extension_list(X509_EXTENSIONS* ext_list, bool acquire);

    size_t size() const noexcept;
    x509_extension operator[](size_t) const;

    x509_extension_iterator begin() noexcept;
    x509_extension_iterator end() noexcept;

private:
    X509_EXTENSIONS* m_ext_list;
    bool m_acquired;
};


