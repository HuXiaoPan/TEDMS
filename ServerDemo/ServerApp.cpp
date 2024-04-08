#include <iostream>
#include "Global.h"
#include "logger.h"
#include "datetime.h"
#include "ServersManager.h"
#include <unistd.h>

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
    
    
    int i = 0;
    int pre = -1;
    while (true)
    {
        utils::Datetime datatime;
        int h = datatime.hour();
        if(pre == 23 && h == 0)i=0;
        pre = h;
       
        LOG_INFO("%s%d%s\n", "[次数报告：",++i,"]Server is running!");
         sleep(2);
    }
    
    
    LOG_INFO("Server is over!!!\n");
    return 0;
}
