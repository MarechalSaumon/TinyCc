cmake_minimum_required(VERSION 3.16.3)
project(TinyCc)

add_compile_options(-Wall -Wextra -Werror -pedantic -Wold-style-cast)

set(CMAKE_CXX_STANDARD 20)

add_executable(TinyCc)

target_include_directories(TinyCc PUBLIC include)
target_include_directories(TinyCc PRIVATE src)

add_subdirectory("src")
