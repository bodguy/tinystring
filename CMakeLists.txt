cmake_minimum_required(VERSION 3.10)
project(utf LANGUAGES CXX VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 14)

set(THIRD_PARTY_LIB_DIRS "third_party/")
include_directories(${THIRD_PARTY_LIB_DIRS})

set(SOURCE_PREFIX "src")
file(GLOB_RECURSE SOURCE_FILES ${SOURCE_PREFIX}/*.h ${SOURCE_PREFIX}/*.cpp)
add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})

# unit test target
find_package(Catch2 REQUIRED)
set(PROJECT_UNIT_TEST_NAME "${CMAKE_PROJECT_NAME}_unit_test")
set(PROJECT_TEST_DIR "${CMAKE_SOURCE_DIR}/test")
file(GLOB_RECURSE TEST_SOURCE_FILES "${PROJECT_TEST_DIR}/*.cpp")

add_executable(${PROJECT_UNIT_TEST_NAME} ${TEST_SOURCE_FILES})
target_link_libraries(${PROJECT_UNIT_TEST_NAME} Catch2::Catch2)

include(CTest)
include(Catch)
catch_discover_tests(${PROJECT_UNIT_TEST_NAME})