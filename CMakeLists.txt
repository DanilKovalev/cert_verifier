cmake_minimum_required(VERSION 3.5)
project(openssl)

set(CMAKE_CXX_STANDARD 14)
add_definitions(-Weffc++ -Wall -Wextra -Wunused-parameter -Wstrict-overflow=5 -Wundef -Wshadow -Wcast-qual -Wcast-align -Wconversion
        -Wsign-conversion  -Wredundant-decls -Wdisabled-optimization -Woverloaded-virtual -Wctor-dtor-privacy -Wnon-virtual-dtor
        -Wold-style-cast -Wlogical-op -Wunused-result)

set ("${PROJECT}_BINARY_DIR"  out)
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/${${PROJECT}_BINARY_DIR})
set (TEST_OUTPUT_PATH ${EXECUTABLE_OUTPUT_PATH}/test)
set (EXAMPLE_OUTPUT_PATH ${EXECUTABLE_OUTPUT_PATH}/example)
set (DOC_OUTPUT_PATH ${EXECUTABLE_OUTPUT_PATH}/doc)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/out/lib)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/out/lib)


enable_testing()
add_subdirectory(src)
add_subdirectory(test)
