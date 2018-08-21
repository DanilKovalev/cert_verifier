include_directories(./)

file(GLOB_RECURSE SOURCE_FILES
        "*.cpp"
        "*.h")

add_library(my_ssl ${SOURCE_FILES} x509/X509VerifyParam.cpp x509/X509VerifyParam.h)
target_link_libraries(my_ssl -lcrypto  -lssl -lcurl)
