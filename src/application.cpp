#include <QDebug>

#include "application.h"
#include "ros2thread.h"


Application* Application::m_singlenApplication = nullptr;

Application::Application(int &argc, char **argv)
    : QApplication(argc,argv)
{
    init();
}

Application::~Application()
{
    if(m_rosThread)
    {
        m_rosThread->quit();
        m_rosThread->wait();
    }
}

Application *Application::instance(int &argc, char **argv)
{
    if(m_singlenApplication == nullptr)
    {
        m_singlenApplication = new Application(argc,argv);
    }

    return m_singlenApplication;
}

void Application::init()
{
    // 启动 ROS2 线程
    if(!m_rosThread)
    {
        m_rosThread = new Ros2Thread(this);
    }

    m_rosThread->start();
}
