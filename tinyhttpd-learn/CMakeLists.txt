cmake_minimum_required(VERSION 3.15)
project(tinyhttpd_learn)

set(CMAKE_CXX_STANDARD 14)

SET(ROOT_DIR D:\\LinuxShare)

# 头文件路径
include_directories(${ROOT_DIR}/include)
# 库文件路径
link_directories(${ROOT_DIR}/lib)
link_libraries(lib_shared)

add_executable(tinyhttpd_learn main.cpp httpd.c simpleclient.c)

# 链接库
target_link_libraries(tinyhttpd_learn lib_shared)