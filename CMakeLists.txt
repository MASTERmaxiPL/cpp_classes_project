cmake_minimum_required(VERSION 3.10)
project(football_management_system LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/2461743991f9aa53e9a3625eafcbacd81a3c74cd.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

set(DOMAIN_SOURCES
        src/stadium/StadiumManager.cpp
        src/match/MatchManager.cpp
        src/club/ClubManager.cpp
)

add_library(domain STATIC ${DOMAIN_SOURCES})
target_include_directories(domain PUBLIC ${CMAKE_SOURCE_DIR}/src)

add_executable(football_management_system src/main.cpp)
target_link_libraries(football_management_system PRIVATE domain)

enable_testing()
include(GoogleTest)

file(GLOB_RECURSE ALL_TEST_FILES "${CMAKE_SOURCE_DIR}/test/*/*Test.cpp")
foreach(test_src IN LISTS ALL_TEST_FILES)
    get_filename_component(test_name ${test_src} NAME_WE) # np. ClubManagerTest
    add_executable(${test_name} ${test_src})
    target_link_libraries(${test_name} PRIVATE domain GTest::gtest_main)
    gtest_discover_tests(${test_name})
endforeach()
