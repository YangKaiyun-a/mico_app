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
    void sigSendNavGoal(const basic::RobotPose &pose);
    void sigPub2DPose(const basic::RobotPose &pose);
    void sigPub2DGoal(const basic::RobotPose &pose);
    void sigTaskFinish();
    void sigTopologyMapUpdate(const TopologyMap &map);
    void sigCurrentSelectPointChanged(const TopologyMap::PointInfo &);
    void sigCursorPose(QPointF pose);


private:
    static SignalManager* m_signalManager;
};

#endif // SIGNALMANAGER_H
