CMAKE_MINIMUM_REQUIRED(VERSION 3.20)
PROJECT(practice)

SET(CMAKE_CXX_STANDARD 23)

# 定义工程根目录; CMAKE_SOURCE_DIR为内建变量，表示工程根目录的CMakeLists.txt文件路径
SET(ROOT_DIR ${CMAKE_SOURCE_DIR})
SET(TARGET_NAME 475)

# 第三方库的目录
SET(LIB_DIR ${ROOT_DIR}/../../tools/out/)

# 指定头文件搜索路径
INCLUDE_DIRECTORIES(${ROOT_DIR}/include)

# 指定引用的外部库的搜索路径
LINK_DIRECTORIES(${LIB_DIR}/lib)

# 构建可执行程序
ADD_EXECUTABLE(${TARGET_NAME} ${TARGET_NAME}.cpp)

# 跨平台配置
IF (WIN32)
    IF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        TARGET_LINK_LIBRARIES(${TARGET_NAME} tools.lib)
        MESSAGE("this is msvc")
    ELSEIF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        TARGET_LINK_LIBRARIES(${TARGET_NAME} tools.lib)
        MESSAGE("this is clang")
    ENDIF ()
ELSEIF (UNIX)
    TARGET_LINK_LIBRARIES(${TARGET_NAME} libtools.a)
ENDIF ()
