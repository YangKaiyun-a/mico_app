#include <QDebug>

#include "mynode.h"

MyNode::MyNode(QObject *parent) : QObject(parent), rclcpp::Node("my_node")
{
    // 创建 publisher
    m_publisher = this->create_publisher<std_msgs::msg::String>("chatter", 10);

    // 创建 subscriber
    m_subscription = this->create_subscription<std_msgs::msg::String>(
        "chatter", 10,
        std::bind(&MyNode::topicCallback, this, std::placeholders::_1)
    );
}

void MyNode::publishText(const QString &text)
{
    if (!m_publisher)
    {
        return;
    }

    auto msg = std_msgs::msg::String();
    msg.data = text.toStdString();
    m_publisher->publish(msg);
}

void MyNode::topicCallback(const std_msgs::msg::String::SharedPtr msg)
{
    QString str = QString::fromStdString(msg->data);
    qDebug() << "Received: " << str;

    // 转成 QString 后通过 Qt 信号发出去
    emit messageReceived(QString::fromStdString(msg->data));
}
