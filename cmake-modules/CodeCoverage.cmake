function(APPEND_COVERAGE_COMPILER_FLAGS TARGET_NAME)
    set(options FORCE)
    set(oneValueArgs OUTPUT_DIR)
    set(multiValueArgs NONE)
    cmake_parse_arguments(PARSE_ARGV 1 COVERAGE "${options}" "${oneValueArgs}" "${multiValueArgs}")

    if (NOT ADD_COVERAGE AND NOT COVERAGE_FORCE)
        return()
    endif()

    if (COVERAGE_OUTPUT_DIR)
        set(COVERAGE_OUTPUT_DIRECTORY ${COVERAGE_OUTPUT_DIR})
    endif()

    if(NOT TARGET_NAME)
        message(FATAL_ERROR "Target is not set")
    endif()

    target_compile_options(${TARGET_NAME} PRIVATE --coverage -fprofile-dir=${COVERAGE_OUTPUT_DIRECTORY}/data)
    target_link_libraries(${TARGET_NAME} PRIVATE -lgcov)
    message(STATUS "Appending code coverage compiler flags for target: ${TARGET_NAME}")

endfunction() # APPEND_COVERAGE_COMPILER_FLAGS