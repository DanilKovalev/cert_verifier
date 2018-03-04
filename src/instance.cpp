#include "instance.h"

#include "openssl/crypto.h"
#include "openssl/ssl.h"
#include "openssl/err.h"
#include "openssl/rand.h"


instance &instance::get()
{
    static instance inst;
    return inst;
}

instance::instance()
: m_locks()
{
    SSL_load_error_strings ();
    SSL_library_init ();
    OpenSSL_add_all_algorithms ();
    ERR_load_crypto_strings();
    RAND_status();

    if (CRYPTO_get_locking_callback() == NULL)
        CRYPTO_set_locking_callback(instance::callback);
}

instance::~instance()
{
    ERR_free_strings ();
    RAND_cleanup ();
    EVP_cleanup ();
    ERR_remove_state (0);
    CRYPTO_cleanup_all_ex_data();
    SSL_COMP_free_compression_methods();
}

void instance::callback(int mode, int n, const char *file, int line)
{
    (void) file;
    (void) line;
    get().lock(static_cast<bool>( mode & CRYPTO_LOCK), n);
}

void instance::lock(bool lock, int lock_id)
{
    if (m_locks.count(lock_id))
        m_locks[lock_id] = std::make_unique<std::mutex>();

    if (lock)
        m_locks[lock_id]->lock();
    else
        m_locks[lock_id]->unlock();
}
