#pragma once

#include <iostream>

#include <openssl/bio.h>

class bio_ostring
{
public:
    bio_ostring();
    bio_ostring(bio_ostring&) = delete;
    bio_ostring& operator =(const bio_ostring&) = delete;

    ~bio_ostring();

    BIO* get_bio();

    const std::string& get_string() const;
    std::string detach_string();

    void destroy();

private:
    BIO* init_bio();
    static BIO_METHOD* init_bio_method();

private:
    static int  s_write( BIO* pBio, const char* pData, int dataLen );
    static int  s_puts( BIO* pBio, const char* pStr );
    static long s_ctrl( BIO* pBio, int cmd, long num, void *ptr );
    static int  s_create( BIO* pBio );
    static int  s_destroy( BIO* pBio );

private:
    BIO_METHOD* m_bioMethod;
    BIO* m_bio;
    std::string m_str;
};

