#include "bio_guards.h"
#include "SslException.h"


static void bio_destroyer(BIO *bio)
{
    if (bio == nullptr)
        return;

    if (BIO_free(bio) != 1)
        throw SslException("Failed to BIO_free");
}

static void bioMethod_destroyer(BIO_METHOD *meth)
{
    if (meth == nullptr)
        return;

    BIO_meth_free(meth);
}

bioPtr createBioGuard(BIO *bio)
{
    return bioPtr(bio, bio_destroyer);
}

bioMethodPtr createBioMethodGuard(BIO_METHOD *method)
{
    return bioMethodPtr(method, bioMethod_destroyer);
}


