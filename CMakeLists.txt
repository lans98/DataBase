cmake_minimum_required(VERSION 3.9)
project(SCPPDB)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(src/include)

# Source
set(SOURCES src/main.cpp)

# Executables
add_executable(scppdb ${SOURCES})

# Executables test
add_executable(block_manager test/block_manager.cpp)
