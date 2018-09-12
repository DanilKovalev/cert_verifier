#include "bio_istring.h"

#include "bio_guards.h"
#include "SslException.h"

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
    }
    catch (std::exception& ex)
    {
        std::cerr << "Failed to destroy bio_istring: " << ex.what() << std::endl;
    }
}

size_t  bio_istring::get_line(char *s, size_t nRead, char delimiter)
{
    if (nRead == 0)
        return 0;

    nRead--; //for '\0'
    size_t lineLength = 0;
    for(size_t i = 0; i < nRead; i++)
    {
        if ((*m_pStr)[m_offset + i] == delimiter)
        {
            lineLength = i + 1;
            break;
        }
    }

    nRead = read(s, lineLength);
    s[nRead] = '\0';

    return nRead;
}

size_t bio_istring::read(char *s, size_t nRead)
{
    if (nRead > m_pStr->size() - m_offset)
        nRead = m_pStr->size() - m_offset + 1;

    memcpy(s, m_pStr->data() + m_offset, nRead);
    m_offset += nRead;
    return nRead;
}

BIO* bio_istring::raw()
{
    return m_bio;
}

BIO* bio_istring::init_bio()
{
    BIO* pBio = BIO_new(getBioMethod());

    if(pBio == nullptr)
        throw SslException("BIO_new");

    BIO_set_data(pBio, static_cast<void*>(this));
    BIO_set_init(pBio, 1);
    return pBio;
}

BIO_METHOD* bio_istring::getBioMethod()
{
    static bioMethodPtr method = createBioMethodGuard(nullptr);
    if(method)
        return method.get();

    method = createBioMethodGuard(BIO_meth_new(BIO_TYPE_SOURCE_SINK, "bio_istring"));
    if(!method)
        throw  SslException("BIO_meth_new");

    BIO_meth_set_read(method.get(), bio_istring::s_read);
    BIO_meth_set_gets(method.get(), bio_istring::s_gets);
    BIO_meth_set_ctrl(method.get(), bio_istring::s_ctrl);
    BIO_meth_set_create(method.get(), bio_istring::s_create);
    BIO_meth_set_destroy(method.get(), bio_istring::s_destroy);

    return method.get();
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

