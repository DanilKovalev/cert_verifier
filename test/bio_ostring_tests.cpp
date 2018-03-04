#include <catch.hpp>
#include <bio/bio_ostring.h>
#include <boost/numeric/conversion/cast.hpp>

TEST_CASE( "bio ostring test", "[bio]" )
{
    bio_ostring bio;
    std::string test_str = "test";

    BIO_write(bio.get_bio(), test_str.c_str(), boost::numeric_cast<int>( test_str.size() ));

    std::string result;
    SECTION("default result")
    {
        result = bio.get_string();
    }
    SECTION("detach result")
    {
        result = bio.detach_string();
    }

    REQUIRE(result == test_str);
}

