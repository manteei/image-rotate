cmake_minimum_required(VERSION 3.23)
project(popitka_2 C)

set(CMAKE_C_STANDARD 99)

add_executable(popitka_2 solution/src/main.c solution/src/bmp.c solution/include/bmp.h solution/src/fileManager.c solution/include/fileManager.h solution/src/picture.c solution/include/picture.h)
