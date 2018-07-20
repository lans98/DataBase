cmake_minimum_required(VERSION 3.9)
project(SCPPDB)

enable_testing()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(src/include)
include_directories(test)
include_directories(dependencies/toml)

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

# Concurrency tests
#add_executable(lock_manager test/lock_manager.cpp)

# Address tests
add_executable(write_address test/address/write.cpp)
add_executable(read_address test/address/read.cpp)

# Entity tests
add_executable(field test/entity/field.cpp)
add_executable(record test/entity/record.cpp)
add_executable(table test/entity/table.cpp)

# Core tests
add_executable(data_type test/core/data_type.cpp)

# Config test
add_executable(config test/config.cpp)

# B+ test
add_executable(bplus test/core/bplus.cpp)
# ======================================================

# ======================================================
# Tests
# ======================================================

# Concurrency tests
#add_test(lock_manager_test lock_manager)

# Address tests
add_test(write_address_test write_address)
add_test(read_address_test read_address)

# Entity tests
add_test(field_test field)
add_test(record_test record)
add_test(table_test table)

# Core tests
add_test(data_type_test data_type)

# Config test
add_test(config_test config)

# B+ test
add_test(bplus_test bplus)
# ======================================================
