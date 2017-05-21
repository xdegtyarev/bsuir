cmake_minimum_required(VERSION 3.7)
project(ipr1)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp des.cpp des.h gost.cpp gost.h des_consts.h utility.h)
add_executable(ipr1 ${SOURCE_FILES})