#include "bio_istring.h"

#include <boost/numeric/conversion/cast.hpp>

#include <sstream>
#include <iostream>
#include <cstring>

bio_istring::bio_istring(const std::string *pStr)
 : m_pBio(init_bio())
 , m_pStr(pStr)
 , m_offset(0)
{
}

bio_istring::~bio_istring()
{
    if(!m_pBio)
        return;

    try
    {
       destroy();
    }
    catch (std::exception& ex)
    {
        std::cerr << "Failed to destroy bio_istring: " << ex.what() << std::endl;
    }
}

size_t  bio_istring::getline(char *s, size_t nRead, char delim)
{
    size_t length = m_pStr->size() - m_offset;

    if(nRead - 1 <  length)
        length = nRead - 1;

    for(size_t i = 0; i < length; i++)
    {
        if ((*m_pStr)[m_offset + i] == delim)
        {
            length = i + 1;
            break;
        }
    }

    nRead = read(s, length);
    if(nRead > 0)
        s[nRead] = '\0';

    return nRead;
}

size_t bio_istring::read(char *s, size_t nRead)
{
    if (nRead + m_offset > m_pStr->size())
        nRead = m_pStr->size() - m_offset + 1;

    memcpy(s, m_pStr->data() + m_offset, nRead);
    m_offset += nRead;
    return nRead;
}

BIO* bio_istring::get_bio()
{
    return m_pBio;
}

void bio_istring::destroy()
{
    if ( BIO_free(m_pBio) != 1)
        std::__throw_runtime_error("Failed to BIO_free");

    m_pBio = nullptr;
}

BIO* bio_istring::init_bio()
{
    static BIO_METHOD methods = {
            BIO_TYPE_SOURCE_SINK,
            "bio_istring",
            nullptr,
            s_read,
            nullptr,
            s_gets,
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

int bio_istring::s_read( BIO* pBio, char* pBuf, int bufLen )
{
    auto nRead = boost::numeric_cast<size_t>(bufLen);
    auto* pBio_str = static_cast<bio_istring*>(pBio->ptr);
    return boost::numeric_cast<int>(pBio_str->read(pBuf, nRead));
}

int bio_istring::s_gets( BIO* pBio, char* pBuf, int bufLen )
{
    auto nRead = boost::numeric_cast<size_t>(bufLen);
    auto* pBio_str = static_cast<bio_istring*>(pBio->ptr);
    return boost::numeric_cast<int>(pBio_str->getline(pBuf, nRead));
}

long bio_istring::s_ctrl( BIO* pBio, int cmd, long num, void *ptr )
{
    (void ) ptr;
    (void ) num;

    auto* pBio_str = static_cast<bio_istring*>(pBio->ptr);
    switch (cmd) {
        case BIO_CTRL_RESET:
            return -1;
        case BIO_C_FILE_SEEK:
            pBio_str->m_offset = boost::numeric_cast<size_t >(num);
            return 0;
        case BIO_C_FILE_TELL:
            return boost::numeric_cast<int > (pBio_str->m_offset );
        case BIO_CTRL_FLUSH:
            return 1;
        default:
            return 0;
    }
}

int bio_istring::s_create( BIO* pBio )
{
    if (!pBio)
        return 0;

    pBio->ptr = nullptr;
    pBio->init = 0;
    return 1;
}

int bio_istring::s_destroy( BIO* pBio )
{
    if (!pBio)
        return 0;

    pBio->ptr = nullptr;
    pBio->init = 0;
    pBio->flags = 0;

    return 1;
}

