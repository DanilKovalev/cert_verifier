#pragma once

#include <openssl/bio.h>

#include <memory>

typedef std::shared_ptr<BIO> bio_ptr;

void bio_destroyer(BIO* pBio);
bio_ptr create_bio_guard(BIO *pBio);



