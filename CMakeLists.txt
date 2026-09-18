# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(EchoServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

aux_source_directory(${CMAKE_SOURCE_DIR}/src SRC_FILES)
include_directories(${CMAKE_SOURCE_DIR}/include)

# 生成可执行文件到 bin 目录
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
add_executable(main ${SRC_FILES})
