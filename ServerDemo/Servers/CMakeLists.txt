# ServersLib/CMakeLists.txt
add_library(ServersLib SHARED )

# 将当前目录下的所有源文件保存到SRC_LIST变量中
set(SRC_LIST
    src/ServerBase.cpp
    )   

# UtilsLib/CMakeLists.txt
add_library(ServersLib SHARED ${SRC_LIST})

# 头文件
target_include_directories(ServersLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes)