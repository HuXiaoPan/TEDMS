#ifndef UTILS_CONFIGHELPER_H
#define UTILS_CONFIGHELPER_H

#include <string>

namespace utils
{
    class ConfigHelper
    {
    private:
        /* data */
    public:
        ConfigHelper(/* args */);
        ~ConfigHelper();

        bool ReadCfg(const std::string &filename);
    };
    
    ConfigHelper::ConfigHelper(/* args */)
    {
    }
    
    ConfigHelper::~ConfigHelper()
    {
    }

    // 有一种文件，叫做config文件，也就是配置文件
    // 在本项目中创建一个文件夹cfg
    // 在其中添加一个配置文件test.ini
    // 在本类中实现操作test.ini的功能

    // ReadCfg

    // WriteCfg
    
}

#endif // UTILS_CONFIGHELPER_H