cmake_minimum_required(VERSION 3.17)
project(lab LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(googletest)

add_executable(${PROJECT_NAME}_tests 04-10.cpp)

target_link_libraries(${PROJECT_NAME}_tests PRIVATE
  GTest::gtest_main
  GTest::gmock
)

add_test(
  NAME run_tests
  COMMAND ${PROJECT_NAME}_tests
)
