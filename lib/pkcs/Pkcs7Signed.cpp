#include "Pkcs7Signed.h"

#include <openssl/obj_mac.h>
#include <stdexcept>
#include <openssl/pkcs7.h>

Pkcs7Signed::Pkcs7Signed(Pkcs7&& pkcs7)
: Pkcs7(std::move( pkcs7) )
, m_pkcs7Signed(m_pkcs7->d.sign)
{
    if(Pkcs7::nid() != NID_pkcs7_signed)
        throw std::logic_error("Failed to create Pkcs7Signed because pkcs7 has different type");
}

Pkcs7Signed::Pkcs7Signed(const Pkcs7Signed& other)
 : Pkcs7(Pkcs7::duplicate(other.m_pkcs7), true)
 , m_pkcs7Signed(m_pkcs7->d.sign)
{
    if(Pkcs7::nid() != NID_pkcs7_signed)
        throw std::logic_error("Failed to create Pkcs7Signed because pkcs7 has different type");
}

Pkcs7Signed::Pkcs7Signed(Pkcs7Signed&& other) noexcept
 : Pkcs7(std::move( other) )
 , m_pkcs7Signed(m_pkcs7->d.sign)
{
}

Pkcs7Signed& Pkcs7Signed::operator=(const Pkcs7Signed& other)
{
    if (this == &other)
        return *this;

    Pkcs7Signed temp(other);
    *this = std::move(temp);
    return *this;
}

Pkcs7Signed& Pkcs7Signed::operator=(Pkcs7Signed&& other) noexcept
{
    if (this == &other)
        return *this;

    swap(other);
    return *this;
}

Pkcs7Signed::~Pkcs7Signed()
{
}

StackOf<X509Certificate> Pkcs7Signed::getCertificates() const
{
    return StackOf<X509Certificate>(reinterpret_cast<struct stack_st*>(m_pkcs7Signed->cert), false);
}

void Pkcs7Signed::swap(Pkcs7Signed& other) noexcept
{
    Pkcs7::swap(other);
    std::swap(this->m_pkcs7Signed, other.m_pkcs7Signed);
}


