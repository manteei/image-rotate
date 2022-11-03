cmake_minimum_required(VERSION 3.23)
project(popitka_2 C)

set(CMAKE_C_STANDARD 99)

add_executable(popitka_2 src/main.c elem/bmp.c elem/bmp.h elem/fileManager.c elem/fileManager.h elem/img.c elem/img.h src/read.cuh)
