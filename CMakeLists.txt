cmake_minimum_required(VERSION 3.15)

include(cmake/prelude.cmake)

project(CNM)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED REQUIRED)
set(CMAKE_CXX_EXTENSIONS None)

include(cmake/get_gtest.cmake)
include(cmake/get_spdlog.cmake)

add_subdirectory(cnm) 
