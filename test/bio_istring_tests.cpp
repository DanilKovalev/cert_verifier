#include <catch.hpp>
#include <bio/bio_istring.h>
#include <boost/numeric/conversion/cast.hpp>

TEST_CASE( "bio istring test", "[bio]" )
{
    std::string test_str = "test";
    bio_istring bio(&test_str);

    char BUFFER[1024] = {0};
    BIO_read(bio.get_bio(), BUFFER, 1);
    REQUIRE(BUFFER[0] == 't');
    BIO_read(bio.get_bio(), BUFFER, 1);
    REQUIRE(BUFFER[0] == 'e');
    BIO_read(bio.get_bio(), BUFFER, 1);
    REQUIRE(BUFFER[0] == 's');
    BIO_read(bio.get_bio(), BUFFER, 1);
    REQUIRE(BUFFER[0] == 't');

    BIO_seek(bio.get_bio(), 1);
    BIO_read(bio.get_bio(), BUFFER, 1);
    REQUIRE(BUFFER[0] == 'e');
}

TEST_CASE( "bio istring seek test", "[bio]" )
{
    std::string test_str = "test";
    bio_istring bio(&test_str);

    char BUFFER[1024] = {0};
    BIO_seek(bio.get_bio(), 1);
    BIO_read(bio.get_bio(), BUFFER, 1);
    REQUIRE(BUFFER[0] == 'e');
}

