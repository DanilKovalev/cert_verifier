#pragma once

template<typename ExtType>
std::optional<ExtType> X509ExtensionsStack::findExtension() const noexcept
{
    for (auto iter = this->cbegin(); iter != this->cend(); ++iter)
    {
        if (iter->nid() == ExtType::NID)
            return ExtType(*iter);
    }

    return std::nullopt;
}

template<typename ExtType>
ExtType X509ExtensionsStack::getExtension() const
{
    auto extension = findExtension<ExtType>();
    if (extension)
        return extension;

    throw std::runtime_error("Extension not found");
}
