cmake_minimum_required(VERSION 3.15)
project(socket-demo)

set(CMAKE_CXX_STANDARD 14)

SET(ROOT_DIR D:\\LinuxShare)

# 头文件路径
#include_directories(${ROOT_DIR}/include)
include_directories(${ROOT_DIR})
# 库文件路径
#link_directories(${ROOT_DIR}/lib)
link_directories(${ROOT_DIR})

link_libraries(lib_shared)

add_executable(socket-demo Client.cpp ClientClass.cpp Server.cpp ServerClass.cpp)

# 链接库
target_link_libraries(socket-demo lib_shared)