#include <QDebug>

#include "application.h"


Application* Application::m_singlenApplication = nullptr;

Application::Application(int &argc, char **argv) : QApplication(argc, argv)
{
    init();
}

Application::~Application()
{

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

}
