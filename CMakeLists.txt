cmake_minimum_required(VERSION 3.25)

project(array)

file(GLOB SOURCES sources/*)

add_library(${CMAKE_PROJECT_NAME} SHARED ${SOURCES})
enable_testing()

set(UNIT_TESTS ${CMAKE_PROJECT_NAME}_tests)

add_executable(${UNIT_TESTS} tests/unit_tests.cpp)
add_test(NAME ${UNIT_TESTS} COMMAND ${UNIT_TESTS})

target_link_libraries(${UNIT_TESTS} PRIVATE ${CMAKE_PROJECT_NAME})
target_include_directories(${UNIT_TESTS} PRIVATE sources)


