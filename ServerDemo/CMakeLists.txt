cmake_minimum_required(VERSION 3.10) # CMake依赖最小版本,必写

# set the project name
# project(ServerApp)
# set the project name and version
project(ServerApp VERSION 0.0.1)

# option 表示提供用户可以选择的选项。命令格式为：option(<variable> "description [initial value])。
option(USE_TEST "Use Test lib" ON)

# add the Test library
# add_subdirectory(Test)

if(USE_TEST)
    add_definitions(-DUSE_TEST)
    message(STATUS "Use the testlib")
    add_subdirectory(TestLib)
    list(APPEND EXTRA_LIBS TestLib)
    # list(APPEND EXTRA_INCLUDES ${PROJECT_SOURCE_DIR}/Test)
endif()

add_subdirectory(Utils)
add_subdirectory(Servers)
add_subdirectory(defines)
list(APPEND EXTRA_LIBS UtilsLib)
list(APPEND EXTRA_LIBS ServersLib)

# list(APPEND EXTRA_INCLUDES ${PROJECT_SOURCE_DIR}/Utils/includes/)

#set 命令指定 var_name 变量，用 ${var_name} 获取变量的值。
set(SRC_LIST ServerApp.cpp)

# add the executable
add_executable(${PROJECT_NAME} ${SRC_LIST})

# 依赖库
target_link_libraries(${PROJECT_NAME} PUBLIC ${EXTRA_LIBS})

# 头文件
target_include_directories(${PROJECT_NAME} PUBLIC
                           ${PROJECT_BINARY_DIR}
                           # ${PROJECT_SOURCE_DIR}/Test
                           # ${EXTRA_INCLUDES}
                           )

# 对工程重名为
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES  OUTPUT_NAME "ServerApp.exe")
#cmake 在构建一个新的target 时，会尝试清理掉其他使用这个名字的库
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES CLEAN_DIRECT_OUTPUT 1)

# 这表示将时间戳已指定格式保存到 COMPILE_TIME 变量中。
string(TIMESTAMP COMPILE_TIME %Y%m%d-%H%M%S)

configure_file(defines/Global.h.in Global.h)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/Lib) # 静态库输出位置

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/Lib) # 动态库输出位置

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin) # 可执行文件输出位置

install(TARGETS ${PROJECT_NAME}
                           RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/App/bin    # 可执行文件安装位置
                           LIBRARY DESTINATION ${PROJECT_SOURCE_DIR}/App/lib    # 动态库安装位置
                           ARCHIVE DESTINATION ${PROJECT_SOURCE_DIR}/App/lib)   # 静态库安装位置

set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Wl,-rpath,../lib")

# 设置要安装的目标文件
set(CONFIGS configs/config.ini)

# 定义install规则
install(FILES ${CONFIGS} DESTINATION ${PROJECT_SOURCE_DIR}/App/configs)

# CMAKE_SOURCE_DIR	根源目录
# CMAKE_CURRENT_SOURCE_DIR	如果使用子项目和目录，则为当前源目录。
# PROJECT_SOURCE_DIR	当前 cmake 项目的源目录。
# CMAKE_BINARY_DIR	根二进制文件生成目录。这是运行cmake命令的目录。
# CMAKE_CURRENT_BINARY_DIR	你当前所处的生成目录。
# PROJECT_BINARY_DIR	当前项目的生成目录。

# 背景
# 有时我们的程序会定义一些暂时使用不上的功能和函数，虽然我们不使用这些功能和函数，但它们往往会浪费我们的ROM和RAM的空间。这在使用静态库时，体现的更为严重。有时，我们只使用了静态库仅有的几个功能，但是系统默认会自动把整个静态库全部链接到可执行程序中，造成可执行程序的大小大大增加。

# 参数详解
# 为了解决前面分析的问题，我们引入了标题中的几个参数。GCC链接操作是以section作为最小的处理单元，只要一个section中的某个符号被引用，该section就会被加入到可执行程序中去。因此，GCC在编译时可以使用 -ffunction-sections和 -fdata-sections 将每个函数或符号创建为一个sections，其中每个sections名与function或data名保持一致。而在链接阶段， -Wl,–gc-sections 指示链接器去掉不用的section（其中-wl, 表示后面的参数 -gc-sections 传递给链接器），这样就能减少最终的可执行程序的大小了。

# 我们常常使用下面的配置启用这个功能：

# CFLAGS += -ffunction-sections -fdata-sections
# LDFLAGS += -Wl,--gc-sections
# add_definitions("-ffunction-sections -fdata-sections")
# set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--gc-sections")

