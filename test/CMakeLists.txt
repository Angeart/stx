cmake_minimum_required(VERSION 2.8)

# set googletest root
set(GTEST_ROOT ${PROJECT_SOURCE_DIR}/third_party/googletest/googletest)
# Include files directory
include_directories(${GTEST_ROOT}/include/ ${CMAKE_CURRENT_SOURCE_DIR})
# Library directory
link_directories(${GTEST_ROOT}/build/)
# Generate executable file
add_executable(Test ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp)
# Link libraries
target_link_libraries(Test stx gtest gtest_main pthread)
