#pragma once

#include <memory>
#include "X509Extension.h"
#include "X509ExtensionIterator.h"

class X509ExtensionList
{
public:
    X509ExtensionList(X509_EXTENSIONS* ext_list, bool acquire);

    size_t size() const noexcept;
    X509Extension operator[](size_t) const;

    X509ExtensionIterator begin() noexcept;
    X509ExtensionIterator end() noexcept;

private:
    X509_EXTENSIONS* m_extList;
    bool m_acquired;
};


