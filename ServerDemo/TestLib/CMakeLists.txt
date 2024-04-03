# TestLib/CMakeLists.txt
add_library(TestLib SHARED TestLib.cpp)

# TestLib/CMakeLists.txt
target_include_directories(TestLib
          INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
          )

install(TARGETS TestLib
                           RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/App/bin    # 可执行文件安装位置
                           LIBRARY DESTINATION ${PROJECT_SOURCE_DIR}/App/bin    # 动态库安装位置
                           ARCHIVE DESTINATION ${PROJECT_SOURCE_DIR}/App/bin)   # 静态库安装位置