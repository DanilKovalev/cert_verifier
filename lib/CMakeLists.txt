file(GLOB_RECURSE SOURCE_FILES
        "*.cpp"
        "*.h")

add_library(my_ssl ${SOURCE_FILES})
target_link_libraries(my_ssl -lcrypto  -lssl -lcurl)
target_include_directories(my_ssl PUBLIC ./)