cmake_minimum_required(VERSION 3.10)
project(LibbitcoinBenchmark VERSION 0.1)
list( APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/modules" )

configure_file(libbitcoin_benchmark_config.hpp.in libbitcoin_benchmark_config.hpp)

find_package( PkgConfig REQUIRED )
# Find google benchmark
find_package( benchmark REQUIRED )
find_package( Bitcoin-System 4.0.0 REQUIRED )
find_package( Bitcoin-Database 4.0.0 REQUIRED )

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

link_directories(bm PUBLIC
  ${bitcoin_system_LIBRARY_DIRS}
  ${bitcoin_database_LIBRARY_DIRS}
  )

add_executable(bm
  src/transaction_manager_benchmark.cpp
  )

target_include_directories(bm PUBLIC
  "${PROJECT_BINARY_DIR}"
  "include"
  ${bitcoin_system_INCLUDE_DIRS}
  ${bitcoin_database_INCLUDE_DIRS}
  )

target_link_libraries(bm PUBLIC
  "benchmark"
  ${bitcoin_system_LIBRARIES}
  ${bitcoin_database_LIBRARIES}
  )
