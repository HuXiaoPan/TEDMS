#include <iostream>
#include "Global.h"

#ifdef USE_TEST
    #include "TestLib.h"
#endif

int main(int argc, char const *argv[])
{
    std::cout << "ServerApp is running!" << std::endl;

#ifdef USE_TEST
    TestFunc();
#else
    std::cout << "Test OFF!" << std::endl;
#endif
    // report time stamp
    std::cout << " Time stamp " << TIMESTAMP << std::endl;

    // report version
    std::cout << " Version " << Tutorial_VERSION_MAJOR << "." << Tutorial_VERSION_MINOR << "." << Tutorial_VERSION_PATCH << std::endl;
    return 0;
}
