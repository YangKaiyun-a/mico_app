#include <QDebug>

#include "application.h"
#include "signalmanager.h"
#include "channel_manager.h"
#include "define.h"


Application* Application::m_singlenApplication = nullptr;

Application::Application(int &argc, char **argv) : QApplication(argc, argv)
{

}

Application::~Application()
{

}

Application *Application::instance()
{
    return m_singlenApplication;
}

Application *Application::instance(int &argc, char **argv)
{
    if(m_singlenApplication == nullptr)
    {
        m_singlenApplication = new Application(argc, argv);
    }

    return m_singlenApplication;
}

void Application::init()
{
    SigManager;

    // 开启 ROS2
    ChannelManager::instance();
    if (ChannelManager::instance()->OpenChannelAuto())
    {
        // 注册回调函数，rclcomm 类中接收到消息后触发信号
        ChannelManager::instance()->RegisterOnDataCallback(std::move([this](const MsgId &id, const std::any &data) {
            Q_EMIT SigManager->sigRecvChannelData(id, data);
        }));
    }

}
