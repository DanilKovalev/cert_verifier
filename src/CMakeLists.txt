include_directories(${PROJECT_SOURCE_DIR}/lib)

file(GLOB_RECURSE SOURCE_FILES
        "*.cpp"
        "*.h")

add_executable(certverify ${SOURCE_FILES})
target_link_libraries(certverify my_ssl -lboost_program_options )
