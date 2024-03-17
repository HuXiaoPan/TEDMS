#include <iostream>
#include "Test.h"
#include "TestLib.h"

#ifdef USE_TEST
    #include "MathFunctions.h"
#endif

int func1(int a, int b);
int func2(int a, int b);

int main(int argc, char const *argv[])
{

#ifdef USE_TEST
    std::cout << "Test ON!" << std::endl;
#else
    std::cout << "Test OFF!" << std::endl;
#endif
    Nothing();
    int a = func1(8,28);
    int b = func2(753,55);
    std::cout << a << "Server is running!" << b << std::endl;

    // report time stamp
    std::cout << " Time stamp " << TIMESTAMP << std::endl;

    // report version
    std::cout << " Version " << Tutorial_VERSION_MAJOR << "." << Tutorial_VERSION_MINOR << "." << Tutorial_VERSION_PATCH << std::endl;
    return 0;
}
