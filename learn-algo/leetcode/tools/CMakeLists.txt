cmake_minimum_required(VERSION 3.20)
project(toolsProject)

set(CMAKE_CXX_STANDARD 23)

# 打印信息
message(STATUS "src This is BINARY dir " ${PROJECT_BINARY_DIR})
message(STATUS "src This is SOURCE dir " ..)

# 指定输出的目录
SET(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/out/)

aux_source_directory(../../../../../../../GitHub/old/leetcode-commit/c++/tools DIR_SRCS)

# 添加 tool-lib 子目录
add_subdirectory(tools-lib)

# 指定生成目标
add_executable(tools-t ${DIR_SRCS})

# 添加链接库
target_link_libraries(tools-t tools)