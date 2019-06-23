function(GENERATE_COVERAGE TARGET_NAME)
    set(options FORCE)
    set(multiValueArgs NONE)
    cmake_parse_arguments(PARSE_ARGV 1 COVERAGE "${options}" "${oneValueArgs}" "${multiValueArgs}")

    if (NOT ADD_COVERAGE AND NOT COVERAGE_FORCE)
        return()
    endif ()
    set(COVERAGE_GENERATOR ${TARGET_NAME}_coverage_report)

    add_custom_target(${COVERAGE_GENERATOR}
            COMMAND "rm" "-rf" "${COVERAGE_OUTPUT_DIRECTORY}/${TARGET_NAME}"
            COMMAND "mkdir" "-p" "${COVERAGE_OUTPUT_DIRECTORY}/${TARGET_NAME}"
            COMMAND "gcovr" "--html" "--delete" "--sort-percentage" "--html-details" "--root" "${CMAKE_SOURCE_DIR}" "-o" "${COVERAGE_OUTPUT_DIRECTORY}/${TARGET_NAME}/index.html"
            )
endfunction()

function(APPEND_COVERAGE_COMPILER_FLAGS TARGET_NAME)
    set(options FORCE)
    set(oneValueArgs OUTPUT_DIR)
    set(multiValueArgs NONE)
    cmake_parse_arguments(PARSE_ARGV 1 COVERAGE "${options}" "${oneValueArgs}" "${multiValueArgs}")

    if (NOT ADD_COVERAGE AND NOT COVERAGE_FORCE)
        return()
    endif()

    if(NOT TARGET_NAME)
        message(FATAL_ERROR "Target is not set")
    endif()

    #target_compile_options(${TARGET_NAME} PRIVATE --coverage -fprofile-dir=${COVERAGE_OUTPUT_DIRECTORY})
    target_compile_options(${TARGET_NAME} PRIVATE --coverage)
    target_link_libraries(${TARGET_NAME} PRIVATE -lgcov)
    message(STATUS "Appending code coverage compiler flags for target: ${TARGET_NAME}")

endfunction() # APPEND_COVERAGE_COMPILER_FLAGS