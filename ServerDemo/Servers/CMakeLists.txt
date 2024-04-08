# 将当前目录下的所有源文件保存到SRC_LIST变量中
set(SRC_LIST
    src/ServerBase.cpp
    src/ServersManager.cpp
    )

list(APPEND EXTRA_LIBS defines)
list(APPEND EXTRA_LIBS UtilsLib)

# UtilsLib/CMakeLists.txt
add_library(ServersLib SHARED ${SRC_LIST})

# 头文件
target_include_directories(ServersLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes)

# 依赖库
target_link_libraries(ServersLib PUBLIC ${EXTRA_LIBS})

install(TARGETS ServersLib
                           RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/App/bin    # 可执行文件安装位置
                           LIBRARY DESTINATION ${PROJECT_SOURCE_DIR}/App/lib    # 动态库安装位置
                           ARCHIVE DESTINATION ${PROJECT_SOURCE_DIR}/App/lib)   # 静态库安装位置