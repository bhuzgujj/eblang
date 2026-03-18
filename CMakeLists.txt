cmake_minimum_required(VERSION 4.1)
project(eblang C)

set(CMAKE_C_STANDARD 23)

add_executable(eblang-test tests/runner.c)
add_executable(eblang src/main.c)
