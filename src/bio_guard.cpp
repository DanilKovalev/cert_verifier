#include "bio_guard.h"

void bio_destroyer(BIO *pBio)
{
    if (pBio == nullptr)
        return;

    if (BIO_free(pBio) != 1)
        std::__throw_runtime_error("Failed to BIO_free");
}

bio_ptr create_bio_guard(BIO *pBio)
{
    return bio_ptr(pBio, bio_destroyer);
}
