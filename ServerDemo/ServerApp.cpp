#include <iostream>
#include "Global.h"
#include "logger.h"
#include "ServersManager.h"

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
    // 先读配置文件，把Log等级读出来给log设置
    utils::Logger::instance()->setLevel(utils::Logger::LEN);

    // 构造并初始化Server
    ServersManager::GetInstance();

    // report time stamp
    LOG_DEBUG("Time stamp %s\n", TIMESTAMP);

    // report version
    LOG_DEBUG(" Version %d.%d.%d\n", Tutorial_VERSION_MAJOR,Tutorial_VERSION_MINOR,Tutorial_VERSION_PATCH);
    while (true)
    {
        LOG_INFO("%s\n", "Server is running!");
    }
    
    
    LOG_INFO("Server is over!!!\n");
    return 0;
}
