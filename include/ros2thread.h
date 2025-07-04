#pragma once
#include <QThread>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

#include "mynode.h"

class Ros2Thread : public QThread
{
    Q_OBJECT
public:
    explicit Ros2Thread(QObject *parent = nullptr);
    ~Ros2Thread() override;

protected:
    void run() override;

private:
    std::shared_ptr<MyNode> m_node;
};
