#pragma once

#include <openssl/bio.h>

#include <memory>

typedef std::shared_ptr<BIO> bioPtr;
typedef std::unique_ptr<BIO_METHOD, void(*)(BIO_METHOD*)> bioMethodPtr;

bioPtr createBioGuard(BIO *bio);
bioMethodPtr createBioMethodGuard(BIO_METHOD *method);



