
# 定义一个库目标，但不指定任何源文件
add_library(defines INTERFACE)

# 头文件
target_include_directories(defines INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})