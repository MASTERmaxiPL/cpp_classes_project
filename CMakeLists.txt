cmake_minimum_required(VERSION 3.10)
project(football_management_system)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/2461743991f9aa53e9a3625eafcbacd81a3c74cd.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(SRC_FILES
  src/Pitch/PitchManager.cpp
)

# Create a library for the Pitch code so it can be linked by both the main executable and tests
add_library(pitch STATIC ${SRC_FILES})
# Export include path so consumers can just include <Pitch/PitchManager.h> or "Pitch/PitchManager.h"
target_include_directories(pitch PUBLIC ${CMAKE_SOURCE_DIR}/src)

add_executable(football_management_system
  src/main.cpp
)
# Link the pitch library into the main executable
target_link_libraries(football_management_system PRIVATE pitch)

enable_testing()

add_executable(PitchManagerTest
  test/Pitch/PitchManagerTest.cpp
)
# Link tests with pitch and googletest
target_link_libraries(PitchManagerTest PRIVATE pitch GTest::gtest_main)

include(GoogleTest)

gtest_discover_tests(PitchManagerTest)
