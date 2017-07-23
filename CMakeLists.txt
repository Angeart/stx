cmake_minimum_required(VERSION 2.8)

#set(CMAKE_VERBOSE_MAKEFILE TRUE)
project(stx CXX)

# find boost
#find_package(Boost 1.64.0 REQUIRED)
#message("boost include directory : ${Boost_INCLUDE_DIRS}")

# set version info
set(serial "1.0.0")
set(serial_so "1")

#set include directories
message("project directory : ${PROJECT_SOURCE_DIR}")
include_directories(${PROJECT_SOURCE_DIR} ${Boost_INCLUDE_DIRS})

#set sources
set(stx_sources
stx/test.cpp
)

add_library(stx STATIC ${stx_sources})
set_target_properties(stx PROPERTIES VERSION ${serial} SOVERSIN ${serial_so})

add_subdirectory(third_party/googletest)
add_subdirectory(test)
enable_testing()
add_test(NAME stx_test COMMAND Test)
add_custom_target(run_test ALL COMMAND "./test/Test" DEPENDS Test gtest gtest_main gmock gmock_main)