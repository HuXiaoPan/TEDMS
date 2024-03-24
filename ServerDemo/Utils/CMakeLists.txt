# find(GLOB_RECURSE SRC_LIST src/*.cpp)

# set(SRC_LIST src/*.cpp)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/. SRC_LIST)  # 将当前目录下的所有源文件保存到SRC_LIST变量中

# UtilsLib/CMakeLists.txt
add_library(UtilsLib SHARED ${SRC_LIST})

# 头文件
target_include_directories(UtilsLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes)