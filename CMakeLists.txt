cmake_minimum_required(VERSION 3.0) # 不同的CMake行为不一样,比如3.0必须在第一个CMakeLists.txt就指定编译器,而3.13就不需要
set(TARGET_NAME words-picker)      # 设置目标名字
project (TARGET_NAME)


set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
message(STATUS "module path = ${CMAKE_MODULE_PATH}")


# 指定构建类型
set(CMAKE_BUILD_TYPE "Debug")
#set(CMAKE_BUILD_TYPE Release)

add_subdirectory(src)
