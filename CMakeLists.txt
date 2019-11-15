cmake_minimum_required(VERSION 3.10)

project(LibbitcoinBenchmark VERSION 0.1)

list( APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/modules" )

add_executable(bm
  src/transaction_manager_benchmark.cpp
  )

configure_file(libbitcoin_benchmark_config.hpp.in libbitcoin_benchmark_config.hpp)

target_include_directories(bm PUBLIC
  "${PROJECT_BINARY_DIR}"
  "include"
  )

find_package( PkgConfig REQUIRED )

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package( Bitcoin-Database 4.0.0 REQUIRED )
