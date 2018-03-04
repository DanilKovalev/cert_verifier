#include "private_key.h"
#include "bio_guard.h"

#include <openssl/pem.h>

private_key::private_key(EVP_PKEY *pKey) noexcept
: m_pKey(pKey)
{

}

private_key::~private_key() noexcept
{
    free();
}

private_key::private_key(private_key&& rhs)
: m_pKey(std::exchange(rhs.m_pKey, nullptr))
{}

private_key& private_key::operator=(private_key&& rhs)
{
    m_pKey = std::exchange(rhs.m_pKey, nullptr);
    return *this;
}

EVP_PKEY* private_key::raw() noexcept
{
    return m_pKey;
}

const EVP_PKEY* private_key::raw() const noexcept
{
    return m_pKey;
}

private_key private_key::from_pem(const std::string& pem)
{
    int len = static_cast<int>(pem.size());
    bio_ptr ptrBio =
            create_bio_guard(BIO_new_mem_buf(static_cast<const void *>(pem.c_str()), len));

    if(!ptrBio)
        std::__throw_runtime_error("Failed to create bio mem buffer");

    EVP_PKEY* pRaw_key = nullptr;
    if( !PEM_read_bio_PrivateKey(ptrBio.get(), &pRaw_key, nullptr, nullptr) )
        std::__throw_runtime_error("Failed to read private key");

    return private_key(pRaw_key);
}

std::string private_key::to_pem() const
{
    bio_ptr ptrBio = create_bio_guard(BIO_new(BIO_s_mem()));
    if( !PEM_write_bio_PrivateKey(ptrBio.get(), m_pKey, nullptr, nullptr, 0, 0, nullptr))
        std::__throw_runtime_error("Failed to write private key");

    std::string pem(ptrBio->num_write, '\0');
    BIO_read(ptrBio.get(), &pem[0], static_cast<int>(ptrBio->num_write));

    return pem;
}

void private_key::free()
{
    EVP_PKEY_free(m_pKey);
}

