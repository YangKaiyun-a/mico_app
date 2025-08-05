#ifndef SIGNALMANAGER_H
#define SIGNALMANAGER_H

#include "msg/msg_info.h"
#include "point_type.h"
#include "topology_map.h"

#include <any>
#include <QObject>
#include <QPointF>


class SignalManager : public QObject
{
    Q_OBJECT
public:
    static SignalManager* instance();

private:
    explicit SignalManager(QObject *parent = nullptr);

signals:
    void sigRecvChannelData(const MsgId &id, const std::any &data);
    void sigSendNavGoal(const basic::RobotPose &pose);                  ///< 发送目标坐标（任务列表页面）
    void sigPub2DPose(const basic::RobotPose &pose);                    ///< 发现重定位坐标
    void sigPub2DGoal(const basic::RobotPose &pose);                    ///< 发送目标坐标（设置目标坐标页面）
    void sigTopologyMapUpdate(const TopologyMap &map);
    void sigCurrentSelectPointChanged(const TopologyMap::PointInfo &);
    void sigCursorPose(const std::string &display_name, QPointF pose);
    void sigRobotPoseChanged(const basic::RobotPose &pose);             ///< 重定位坐标发生改变
    void sigTargetChanged(const basic::RobotPose &pose);                ///< 目标点坐标发生改变
    void sigLogin(bool);                                                ///< 用户登录

private:
    static SignalManager* m_signalManager;
};

#endif // SIGNALMANAGER_H
