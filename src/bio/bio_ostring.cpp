#include "bio_ostring.h"

#include <boost/numeric/conversion/cast.hpp>

#include <cstring>

bio_ostring::bio_ostring()
 : m_bioMethod(nullptr)
 , m_bio(init_bio())
 , m_str()
{}

bio_ostring::~bio_ostring()
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
        std::cerr << "Failed to destroy bio_ostring: " << ex.what() << std::endl;
    }
}

BIO* bio_ostring::get_bio()
{
    return m_bio;
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
    if ( BIO_free(m_bio) != 1)
        std::__throw_runtime_error("Failed to BIO_free");

    m_bio = nullptr;
}

BIO_METHOD* bio_ostring::init_bio_method()
{
    BIO_METHOD* method = BIO_meth_new(BIO_TYPE_SOURCE_SINK, "bio_ostring");
    if(method == nullptr)
        std::__throw_runtime_error("BIO_new(&methods)");

    BIO_meth_set_write(method, bio_ostring::s_write);
    BIO_meth_set_puts(method, bio_ostring::s_puts);
    BIO_meth_set_ctrl(method, bio_ostring::s_ctrl);
    BIO_meth_set_create(method, bio_ostring::s_create);
    BIO_meth_set_destroy(method, bio_ostring::s_destroy);

    return method;
}

BIO* bio_ostring::init_bio()
{
    m_bioMethod = init_bio_method();
    BIO* pBio = BIO_new(m_bioMethod);
    if(pBio == nullptr)
        std::__throw_runtime_error("BIO_new(method)");

    BIO_set_data(pBio, static_cast<void*>(this));
    BIO_set_init(pBio, 1);
    return pBio;
}

int bio_ostring::s_write( BIO* pBio, const char* pData, int dataLen )
{
    auto* pBio_str = static_cast<bio_ostring*>(BIO_get_data(pBio));
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

    auto* pBio_str = static_cast<bio_ostring*>(BIO_get_data(pBio));
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

    BIO_set_data(pBio, nullptr);
    BIO_set_init(pBio, 0);
    return 1;
}

int bio_ostring::s_destroy( BIO* pBio )
{
    if (!pBio)
        return 0;

    BIO_set_data(pBio, nullptr);
    BIO_set_init(pBio, 0);
    BIO_set_flags(pBio, 0);

    return 1;
}

