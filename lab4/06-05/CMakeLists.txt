cmake_minimum_required(VERSION 3.10)
project(dynamic_library_loader)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

set(Boost_USE_STATIC_LIBS    ON)
set(Boost_USE_STATIC_RUNTIME ON)

find_package(Boost 1.82.0 REQUIRED COMPONENTS filesystem system)

add_library(v1 SHARED v1.cpp)
target_include_directories(v1 PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(v1 PRIVATE ${Boost_LIBRARIES})

add_library(v2 SHARED v2.cpp)
target_include_directories(v2 PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(v2 PRIVATE ${Boost_LIBRARIES})

add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LIBRARIES})