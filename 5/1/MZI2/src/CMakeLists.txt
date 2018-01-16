cmake_minimum_required(VERSION 3.7)
project(mzi)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp des.cpp des.h gost.cpp gost.h utility.h rsa.cpp rsa.h gost3410.cpp gost3410.h gost3411.cpp gost3411.h caesar.cpp caesar.h aes.cpp aes.h md5.cpp md5.h ecc.cpp ecc.h eccfield.h bitvector.h)
add_executable(mzi ${SOURCE_FILES})