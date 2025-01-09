cmake_minimum_required(VERSION 3.10)
project(SIMD-test C)

set(CMAKE_C_STANDARD 99)

add_executable(simdtest src/main.c)

target_compile_options(simdtest PRIVATE -mavx2)
