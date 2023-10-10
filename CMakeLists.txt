cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

project(CppReflection VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include(CMakeDependentOption)

set(BIN_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin")

add_subdirectory(source)