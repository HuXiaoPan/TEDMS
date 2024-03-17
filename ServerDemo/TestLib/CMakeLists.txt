# MathFunctions/CMakeLists.txt
add_library(TestLib SHARED TestLib.cpp)

# Test/CMakeLists.txt
target_include_directories(TestLib
          INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
          )