cmake_minimum_required(VERSION 3.20)
set(CXX_STANDARD 20)

project(quaternion_calc CXX)

set(LIBS)

add_executable(q_calc src/main.cpp)

# tests
enable_testing()

file(GLOB_RECURSE TEST_SRCS ${CMAKE_SOURCE_DIR}/tests/*.cxx)

foreach(TEST_SRC ${TEST_SRCS})
    get_filename_component(NAME ${TEST_SRC} NAME_WLE)

    add_executable(${NAME} ${TEST_SRC})
    set_target_properties(${NAME} PROPERTIES 
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests/)

    add_test(NAME ${NAME} COMMAND ${NAME})
endforeach()
