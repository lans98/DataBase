cmake_minimum_required(VERSION 3.9)
project(SCPPDB)

enable_testing()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(src/include)
include_directories(test)

# ======================================================
# Source
# ======================================================
set(SOURCES src/main.cpp)
# ======================================================

# ======================================================
# Executables
# ======================================================
add_executable(scppdb ${SOURCES})
# ======================================================

# ======================================================
# Executables test
# ======================================================
#add_executable(lock_manager test/lock_manager.cpp)

# Address tests
add_executable(write_address test/address/write.cpp)
add_executable(read_address test/address/read.cpp)
# ======================================================

# ======================================================
# Tests
# ======================================================
#add_test(lock_manager_test lock_manager)
# ======================================================
