cmake_minimum_required(VERSION 3.14)
project(CppTasks LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB TASK_SOURCES *.cc)

foreach(src_file ${TASK_SOURCES})
    get_filename_component(target_name ${src_file} NAME_WE)
    add_executable(${target_name} ${src_file})
    target_compile_options(${target_name} PRIVATE -Wall -Wextra -Wpedantic)
endforeach()
