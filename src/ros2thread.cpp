#include <QDebug>

#include "ros2thread.h"

Ros2Thread::Ros2Thread(QObject *parent)
    : QThread(parent)
{

}

Ros2Thread::~Ros2Thread()
{
    rclcpp::shutdown();
    wait();
}

void Ros2Thread::run()
{
    qDebug() << "start node thread";

    rclcpp::init(0, nullptr);
    m_node = std::make_shared<MyNode>();
    rclcpp::spin(m_node);
}
