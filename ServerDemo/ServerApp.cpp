#include <iostream>
#include "Global.h"
#include "logger.h"

#ifdef USE_TEST
    #include "TestLib.h"
#endif

int main(int argc, char const *argv[])
{
    LOG_INFO("Server is run!!!\n");

#ifdef USE_TEST
    TestFunc();
#else
    LOG_DEBUG("Test OFF!\n");
#endif
    // report time stamp
    LOG_DEBUG("Time stamp %c\n", TIMESTAMP);

    // report version
    LOG_DEBUG(" Version %f.%f.%f\n", Tutorial_VERSION_MAJOR,Tutorial_VERSION_MINOR,Tutorial_VERSION_PATCH);
    
    LOG_INFO("Server is over!!!\n");
    return 0;
}
