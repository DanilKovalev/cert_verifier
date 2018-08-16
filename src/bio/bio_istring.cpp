#include "bio_istring.h"

#include <boost/numeric/conversion/cast.hpp>

#include <sstream>
#include <iostream>
#include <cstring>

bio_istring::bio_istring(const std::string *pStr)
 : m_bio(init_bio())
 , m_pStr(pStr)
 , m_offset(0)
{
}

bio_istring::~bio_istring()
{
    try
    {
        if(m_bio && BIO_free(m_bio) != 1)
            std::__throw_runtime_error("Failed to BIO_free");

        if(m_bioMethod)
            BIO_meth_free(m_bioMethod);
    }
    catch (std::exception& ex)
    {
        std::cerr << "Failed to destroy bio_istring: " << ex.what() << std::endl;
    }
}

size_t  bio_istring::get_line(char *s, size_t nRead, char delimiter)
{
    size_t length = m_pStr->size() - m_offset;

    if(nRead - 1 <  length)
        length = nRead - 1;

    for(size_t i = 0; i < length; i++)
    {
        if ((*m_pStr)[m_offset + i] == delimiter)
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
    return m_bio;
}

BIO* bio_istring::init_bio()
{
    m_bioMethod = init_bio_method();
    BIO* pBio = BIO_new(m_bioMethod);

    if(pBio == nullptr)
        std::__throw_runtime_error("BIO_new(&methods)");

    BIO_set_data(pBio, static_cast<void*>(this));
    BIO_set_init(pBio, 1);
    return pBio;
}

BIO_METHOD* bio_istring::init_bio_method()
{
    BIO_METHOD* method = BIO_meth_new(BIO_TYPE_SOURCE_SINK, "bio_istring");
    if(method == nullptr)
        std::__throw_runtime_error("BIO_new(&methods)");

    BIO_meth_set_read(method, bio_istring::s_read);
    BIO_meth_set_gets(method, bio_istring::s_gets);
    BIO_meth_set_ctrl(method, bio_istring::s_ctrl);
    BIO_meth_set_create(method, bio_istring::s_create);
    BIO_meth_set_destroy(method, bio_istring::s_destroy);

    return method;
}

int bio_istring::s_read( BIO* pBio, char* pBuf, int bufLen )
{
    auto nRead = boost::numeric_cast<size_t>(bufLen);
    auto* pBio_str = static_cast<bio_istring*>(BIO_get_data(pBio));
    return boost::numeric_cast<int>(pBio_str->read(pBuf, nRead));
}

int bio_istring::s_gets( BIO* pBio, char* pBuf, int bufLen )
{
    auto nRead = boost::numeric_cast<size_t>(bufLen);
    auto* pBio_str = static_cast<bio_istring*>(BIO_get_data(pBio));
    return boost::numeric_cast<int>(pBio_str->get_line(pBuf, nRead));
}

long bio_istring::s_ctrl( BIO* pBio, int cmd, long num, void *ptr )
{
    (void ) ptr;
    (void ) num;

    auto* pBio_str = static_cast<bio_istring*>(BIO_get_data(pBio));
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

    BIO_set_data(pBio, nullptr);
    BIO_set_init(pBio, 0);
    return 1;
}

int bio_istring::s_destroy( BIO* pBio )
{
    if (!pBio)
        return 0;

    BIO_set_data(pBio, nullptr);
    BIO_set_init(pBio, 0);
    BIO_set_flags(pBio, 0);
    return 1;
}

