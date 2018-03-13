include_directories(${PROJECT_SOURCE_DIR}/include)
set(TEST_TARGET test_lib)

file(GLOB_RECURSE SOURCE_FILES
        "*.cpp"
        "*.h")

add_executable(${TEST_TARGET} ${SOURCE_FILES})
target_link_libraries(${TEST_TARGET} my_ssl)

set_target_properties(${TEST_TARGET} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_PATH})

add_test(NAME "cryptoo"
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/test
    COMMAND ${EXECUTABLE_OUTPUT_PATH}/test/${TEST_TARGET}
)

