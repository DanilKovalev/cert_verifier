#include "bio_ostring.h"

#include <boost/numeric/conversion/cast.hpp>

#include <cstring>

bio_ostring::bio_ostring()
 : m_pBio(init_bio())
 , m_str()
{}

bio_ostring::~bio_ostring()
{
    if(!m_pBio)
        return;

    try
    {
       destroy();
    }
    catch (std::exception& ex)
    {
        std::cerr << "Failed to destroy bio_ostring: " << ex.what() << std::endl;
    }
}

BIO* bio_ostring::get_bio()
{
    return m_pBio;
}


const std::string& bio_ostring::get_string() const
{
    return m_str;
}

std::string bio_ostring::detach_string()
{
    return std::move(m_str);
}

void bio_ostring::destroy()
{
    if ( BIO_free(m_pBio) != 1)
        std::__throw_runtime_error("Failed to BIO_free");

    m_pBio = nullptr;
}

BIO* bio_ostring::init_bio()
{
    static BIO_METHOD methods = {
            BIO_TYPE_SOURCE_SINK,
            "bio_ostring",
            s_write,
            nullptr,
            s_puts,
            nullptr,
            s_ctrl,
            s_create,
            s_destroy,
            nullptr,
    };

    BIO* pBio = BIO_new(&methods);
    if(pBio == nullptr)
        std::__throw_runtime_error("BIO_new(&methods)");

    pBio->ptr = static_cast<void*>(this);
    pBio->init = 1;
    return pBio;
}

int bio_ostring::s_write( BIO* pBio, const char* pData, int dataLen )
{
    auto* pBio_str = static_cast<bio_ostring*>(pBio->ptr);
    pBio_str->m_str.append(pData, boost::numeric_cast<size_t>(dataLen));
    return dataLen;
}

int bio_ostring::s_puts(BIO* pBio, const char* pStr)
{
    return s_write( pBio, pStr, int( ::strlen(pStr) ) );
}

long bio_ostring::s_ctrl( BIO* pBio, int cmd, long num, void *ptr )
{
    (void ) ptr;
    (void ) num;

    auto* pBio_str = static_cast<bio_ostring*>(pBio->ptr);
    switch (cmd) {
        case BIO_CTRL_RESET:
            pBio_str->m_str.clear();
            return 1;
        case BIO_C_FILE_SEEK:
            return -1;
        case BIO_C_FILE_TELL:
            return boost::numeric_cast<int > (pBio_str->m_str.size() );
        case BIO_CTRL_FLUSH:
            return 1;
        default:
            return 0;
    }
}

int bio_ostring::s_create( BIO* pBio )
{
    if (!pBio)
        return 0;

    pBio->ptr = nullptr;
    pBio->init = 0;
    return 1;
}

int bio_ostring::s_destroy( BIO* pBio )
{
    if (!pBio)
        return 0;

    pBio->ptr = nullptr;
    pBio->init = 0;
    pBio->flags = 0;

    return 1;
}

