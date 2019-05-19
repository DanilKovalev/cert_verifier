project(SslWrapperLib)

file(GLOB_RECURSE SOURCE_FILES
        "*.cpp"
        "*.h")

add_library(${PROJECT_NAME} ${SOURCE_FILES})


set(ADD_LIBS
        /home/ostik/projects/mirrors/openssl/libssl.a
        /home/ostik/projects/mirrors/openssl/libcrypto.a
        -lpthread
        -ldl
        )

target_link_libraries(${PROJECT_NAME} PUBLIC -lcrypto  -lssl -lcurl)
#target_link_libraries(${PROJECT_NAME} PUBLIC ${ADD_LIBS} -lcurl)

add_library(SslWrapper::lib ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PUBLIC ./)

APPEND_COVERAGE_COMPILER_FLAGS(${PROJECT_NAME})
