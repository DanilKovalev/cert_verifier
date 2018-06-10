#pragma once

#include <openssl/bio.h>

#include <memory>

typedef std::shared_ptr<BIO> bioPtr;

bioPtr createBioGuard(BIO *bio);



