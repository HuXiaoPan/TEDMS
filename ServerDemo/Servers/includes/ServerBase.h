#ifndef SERVERBASE_H
#define SERVERBASE_H

/*服务类基类
功能：  1. 提供服务类通用接口*/

class ServerBase
{
private:
    /* data */
public:
    // 单例实现

    ServerBase(/* args */);
    ~ServerBase();

    // 这里应该有个初始化函数
    virtual void Init() = 0;
    virtual int Input() = 0;
    virtual int Output() = 0;

};
#endif // !SERVERBASE_H