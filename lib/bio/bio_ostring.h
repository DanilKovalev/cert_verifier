#pragma once

#include "bio_guards.h"

#include <openssl/bio.h>
#include <iostream>

class bio_ostring
{
public:
    bio_ostring();
    bio_ostring(const bio_ostring&) = delete;
    bio_ostring& operator =(const bio_ostring&) = delete;

    bio_ostring(bio_ostring&& other) noexcept;
    bio_ostring& operator =(bio_ostring&& other) noexcept;

    void swap(bio_ostring& other);

    ~bio_ostring();

    BIO* get_bio();

    const std::string& get_string() const;
    std::string detach_string();

private:
    BIO* init_bio();
    static BIO_METHOD* getBioMethod();

    void destroy();

private:
    static int  s_write( BIO* pBio, const char* pData, int dataLen );
    static int  s_puts( BIO* pBio, const char* pStr );
    static long s_ctrl( BIO* pBio, int cmd, long num, void *ptr );
    static int  s_create( BIO* pBio );
    static int  s_destroy( BIO* pBio );

private:
    BIO* m_bio;
    std::string m_str;
};

