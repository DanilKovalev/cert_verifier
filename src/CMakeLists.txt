include_directories(${PROJECT_SOURCE_DIR}/lib)

file(GLOB_RECURSE SOURCE_FILES
        "*.cpp"
        "*.h")

add_executable(certverify ${SOURCE_FILES})
target_link_libraries(certverify SslWrapper::lib -lboost_program_options -lboost_system -lboost_filesystem)
