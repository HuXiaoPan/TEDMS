#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include "Common.h"
#include <mutex>
#include "ServerBase.h"
#include <map>
#include <string>

/*服务管理类
功能：  1.构造所有服务类（在构造函数）
        2.初始化所有服务类（在初始化函数）
        3.负责服务类之间沟通（建立push和get等函数）
        4.提供公共数据让服务类使用*/
class ServersManager
{
public:
    // 获取单例对象
    static ServersManager *GetInstance();
    RESULT Init();

private:
    // 为了实现单例，限制其他获取对象的方法
    ServersManager(/* args */);
    ServersManager(const ServersManager &target);
    const ServersManager &operator=(const ServersManager &target);
    ~ServersManager();

    // 唯一单实例对象指针
    static ServersManager *m_SingleInstance;
    static std::mutex m_Mutex;

    std::map<std::string, ServerBase *> m_serversMap;

};
#endif // !SERVERMANAGER_H