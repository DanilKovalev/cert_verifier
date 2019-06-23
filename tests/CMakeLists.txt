include_directories(${PROJECT_SOURCE_DIR}/lib)
set(PROJECT_NAME test_lib)
set(TEST_NAME "unit_tests")

file(GLOB_RECURSE SOURCE_FILES
        "*.cpp"
        "*.h")

add_executable(${PROJECT_NAME} ${SOURCE_FILES} X509StoreCtx_test.cpp)
target_link_libraries(${PROJECT_NAME} SslWrapper::lib)
set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_PATH})

add_test(NAME "${TEST_NAME}"
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/tests/
        COMMAND ${TEST_OUTPUT_PATH}/${PROJECT_NAME})
GENERATE_COVERAGE(${TEST_NAME})
