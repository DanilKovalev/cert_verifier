#include "bio_guard.h"

void bio_destroyer(BIO *bio)
{
    if (bio == nullptr)
        return;

    if (BIO_free(bio) != 1)
        std::__throw_runtime_error("Failed to BIO_free");
}

bioPtr createBioGuard(BIO *bio)
{
    return bioPtr(bio, bio_destroyer);
}
