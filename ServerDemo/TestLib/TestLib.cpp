#include "TestLib.h"
#include <iostream>

void TestFunc()
{
    std::cout << "FileName: " << __FILE__
              << "FuncName: " << __func__
              << "Line: " << __LINE__
              << std::endl;
    return;
}