#pragma once
#include <QObject>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

class MyNode : public QObject, public rclcpp::Node
{
    Q_OBJECT
public:
    explicit MyNode(QObject *parent = nullptr);

    // 供 UI 调用的发布接口（可直接在信号槽里连接）
    Q_INVOKABLE void publishText(const QString &text);

signals:
    // 当订阅到新消息时通知 UI
    void messageReceived(const QString &text);

private:
    void topicCallback(const std_msgs::msg::String::SharedPtr msg);

    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr  m_publisher;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr m_subscription;
};
