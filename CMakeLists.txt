cmake_minimum_required(VERSION 3.20)
set(CXX_STANDARD 20)

project(quaternion_calc CXX)

# Boost
find_package(Boost CONFIG REQUIRED)
set(LIBS ${LIBS} Boost::program_options Boost::container)

# FMT
find_package(fmt CONFIG REQUIRED)
set(LIBS ${LIBS} fmt::fmt)

add_executable(q_calc src/main.cpp)
target_link_libraries(q_calc ${LIBS})

# tests
enable_testing()

find_package(Catch2 CONFIG REQUIRED)

file(GLOB_RECURSE TEST_SRCS ${CMAKE_SOURCE_DIR}/tests/*.cxx)

foreach(TEST_SRC ${TEST_SRCS})
    get_filename_component(NAME ${TEST_SRC} NAME_WLE)

    add_executable(${NAME} ${TEST_SRC})
    set_target_properties(${NAME} PROPERTIES 
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests/)
    target_link_libraries(${NAME} ${LIBS} Catch2::Catch2WithMain)

    add_test(NAME ${NAME} COMMAND ${NAME})
endforeach()
