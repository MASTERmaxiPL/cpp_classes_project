cmake_minimum_required(VERSION 3.10)
project(football_management_system LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/2461743991f9aa53e9a3625eafcbacd81a3c74cd.zip
)
FetchContent_MakeAvailable(googletest)

set(DOMAIN_SOURCES
  src/Pitch/PitchManager.cpp
)

add_library(domain STATIC ${DOMAIN_SOURCES})
target_include_directories(domain PUBLIC ${CMAKE_SOURCE_DIR}/src)

add_executable(football_management_system src/main.cpp)
target_link_libraries(football_management_system PRIVATE domain)

enable_testing()
add_executable(PitchManagerTest test/Pitch/PitchManagerTest.cpp)
target_link_libraries(PitchManagerTest PRIVATE domain GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(PitchManagerTest)
