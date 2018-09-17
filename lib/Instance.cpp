#include "Instance.h"

#include <openssl/crypto.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <curl/curl.h>
#include "http/HttpException.h"


Instance &Instance::get()
{
    static Instance inst;
    return inst;
}

Instance::Instance()
{
    CURLcode code = curl_global_init(CURL_GLOBAL_ALL);
    if(code != CURLE_OK)
        throw HttpException(code);

    SSL_load_error_strings ();
    SSL_library_init ();
    OpenSSL_add_all_algorithms ();
    ERR_load_crypto_strings();
    RAND_status();
}

Instance::~Instance()
{
    curl_global_cleanup();
    ERR_free_strings ();
    RAND_cleanup ();
    EVP_cleanup ();
    CRYPTO_cleanup_all_ex_data();
    //SSL_COMP_free_compression_methods();
}


