cmake_minimum_required(VERSION 3.10)

# set the project name
# project(Server)
# set the project name and version
project(Server VERSION 0.0.1)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/Lib)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/Lib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

# option 表示提供用户可以选择的选项。命令格式为：option(<variable> "description [initial value])。
option(USE_TEST "Use Test lib" ON)

# add the Test library
# add_subdirectory(Test)

if(USE_TEST)
  add_subdirectory(TestLib)
  list(APPEND EXTRA_LIBS TestLib)
  # list(APPEND EXTRA_INCLUDES ${PROJECT_SOURCE_DIR}/Test)
endif()

# 这表示将时间戳已指定格式保存到 COMPILE_TIME 变量中。
string(TIMESTAMP COMPILE_TIME %Y%m%d-%H%M%S)

configure_file(Test.h.in Test.h)

#set 命令指定 var_name 变量，用 ${var_name} 获取变量的值。
#set(SRC_LIST a.cpp b.cpp c.cpp)
set(SRC_LIST a.cpp b.cpp c.cpp)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the executable
add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(${PROJECT_NAME} PUBLIC ${EXTRA_LIBS})

target_include_directories(${PROJECT_NAME} PUBLIC
                           ${PROJECT_BINARY_DIR}
                           # ${PROJECT_SOURCE_DIR}/Test
                           # ${EXTRA_INCLUDES}
                           )

install(TARGETS ${PROJECT_NAME} TestLib
                           RUNTIME DESTINATION ${PROJECT_BINARY_DIR}../../../App/bin
                           LIBRARY DESTINATION ${PROJECT_BINARY_DIR}../../../App/lib
                           ARCHIVE DESTINATION ${PROJECT_BINARY_DIR}../../../App/lib)

# CMAKE_SOURCE_DIR	根源目录
# CMAKE_CURRENT_SOURCE_DIR	如果使用子项目和目录，则为当前源目录。
# PROJECT_SOURCE_DIR	当前 cmake 项目的源目录。
# CMAKE_BINARY_DIR	根二进制文件生成目录。这是运行cmake命令的目录。
# CMAKE_CURRENT_BINARY_DIR	你当前所处的生成目录。
# PROJECT_BINARY_DIR	当前项目的生成目录。
