#include "ServersManager.h"
#include "logger.h"

//初始化静态成员变量
ServersManager *ServersManager::m_SingleInstance = nullptr;
std::mutex ServersManager::m_Mutex;

ServersManager *ServersManager::GetInstance()
{
    //  这里使用了两个 if 判断语句的技术称为双检锁；好处是，只有判断指针为空的时候才加锁，
    //  避免每次调用 GetInstance的方法都加锁，锁的开销毕竟还是有点大的。
    if (m_SingleInstance == nullptr) 
    {
        std::unique_lock<std::mutex> lock(m_Mutex); // 加锁
        if (m_SingleInstance == nullptr)
        {
            m_SingleInstance = new ServersManager();
            LOG_INFO("%s", "ServerManager is construction!!!\n");
        }
    }

    return m_SingleInstance;
}

RESULT ServersManager::Init()
{
    return FAILURE;
}

ServersManager::ServersManager(/* args */)
{
}

ServersManager::ServersManager(const ServersManager &target)
{
}

const ServersManager &ServersManager::operator=(const ServersManager &target)
{
    return target;
}

ServersManager::~ServersManager()
{
}