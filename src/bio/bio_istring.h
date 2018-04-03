#pragma once

#include <openssl/bio.h>

#include <string>

class bio_istring
{
public:
    explicit bio_istring(const std::string* pStr);
    bio_istring(bio_istring&) = delete;
    bio_istring& operator =(const bio_istring&) = delete;

    ~bio_istring();

    BIO* get_bio();

    size_t  get_line(char *s, size_t n, char delimiter = '\n');
    size_t read(char *s, size_t n);

    void destroy();
private:
    BIO* init_bio();

private:
    static int  s_read( BIO* pBio, char* pBuf, int bufLen );
    static int  s_gets( BIO* pBio, char* pBuf, int bufLen );
    static long s_ctrl( BIO* pBio, int cmd, long num, void *ptr );
    static int  s_create( BIO* pBio );
    static int  s_destroy( BIO* pBio );

private:
    BIO* m_pBio = nullptr;
    const std::string* m_pStr = nullptr;
    size_t m_offset = 0;
};

