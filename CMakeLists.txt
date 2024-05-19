cmake_minimum_required(VERSION 3.16)
project(Miyamoto VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20 CACHE STRING "" FORCE)

set(CXX_STANDARD_REQUIRED ON CACHE BOOL "" FORCE)

set(CMAKE_CXX_STANDARD_REQUIRED ON CACHE BOOL "" FORCE)

# Including GoogleTest as a dependency
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  Tests
  src/tests/Tests.cpp
)
target_link_libraries(
  Tests
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(Tests)
#

set(CACHE CMAKE_VERBOSE_MAKEFILE TRUE CACHE BOOL "" FORCE)

set(CMAKE_CXX_COMPILER "g++" CACHE STRING "" FORCE)


file(GLOB_RECURSE MY_SOURCES "src/*.cpp")

foreach(item ${MY_SOURCES}) # to remove Tests from being loaded twice
  if(${item} MATCHES "src/tests/Tests.cpp")
    list(REMOVE_ITEM MY_SOURCES ${item})
  endif()
endforeach()

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -Wall -Wextra -Wpedantic -ggdb" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_RELEASE "-O3  -march=native  -mtune=native" CACHE STRING "" FORCE)

add_executable(Miyamoto ${MY_SOURCES})


install(TARGETS Miyamoto)