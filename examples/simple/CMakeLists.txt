cmake_minimum_required(VERSION 3.12...4.0)
project(hello)

find_package(VexCL)

vexcl_add_executables(hello hello.cpp)
