#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QCalendarWidget>
#include <QComboBox>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QGraphicsItem>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QProgressBar>
#include <QPushButton>
#include <QRadioButton>
#include <QSettings>
#include <QTableWidget>
#include <QToolBar>
#include <QTreeView>
#include <QWidgetAction>
#include <opencv2/imgproc/imgproc.hpp>
#include "DockAreaWidget.h"
#include "DockManager.h"
#include "DockWidget.h"
#include "display/manager/display_manager.h"
#include "point_type.h"
#include "dashboard.h"
#include "nav_goal_table_view.h"
#include "speed_ctrl.h"
#include "ratio_layouted_frame.h"

namespace Ui {
class MapWidget;
}

class MapWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MapWidget(QWidget *parent = nullptr);
    ~MapWidget();
    void init();
    void initUI();
    void SendChannelMsg(const MsgId &id, const std::any &data);             ///< 发送 ROS2 数据
    void SaveState();                                                       ///< 保存当前窗口配置
    void RestoreState();                                                    ///< 恢复上次保存的窗口配置

public slots:
    void onSigCursorPose(const std::string &display_name, QPointF pos);     ///< 接收当前光标的坐标
    void onSigSendNavGoal(const std::any &data);                            ///< 发送目标点位的槽函数
    void onSigPub2DPose(const basic::RobotPose &pose);                      ///< 发送位姿的槽函数
    void onSigPub2DGoal(const basic::RobotPose &pose);                      ///< 发送目标点位的槽函数
    void onSigRecvChannelData(const MsgId &id, const std::any &data);       ///< 对接收到的 ROS2 数据进行分发
    void updateOdomInfo(RobotState state);
    void SlotSetBatteryStatus(double percent, double voltage);
    void SlotRecvImage(const std::string &location, std::shared_ptr<cv::Mat> data);

private:
    Ui::MapWidget *ui;
    ads::CDockManager *m_dock_manager = nullptr;
    DashBoard *m_speed_dash_board = nullptr;                       ///< 速度仪表盘
    ads::CDockAreaWidget *m_statusDockArea = nullptr;
    ads::CDockWidget *m_timelineDockWidget = nullptr;
    Display::DisplayManager *m_display_manager = nullptr;          ///< 管理地图显示
    QLabel *m_label_pos_map = nullptr;                             ///< 坐标
    QLabel *m_label_pos_scene = nullptr;                           ///< 坐标
    QThread m_message_thread;
    SpeedCtrlWidget *m_speed_ctrl_widget = nullptr;                ///< 速度控制页面
    NavGoalTableView *m_nav_goal_table_view = nullptr;             ///< 任务列表页面
    QProgressBar *m_battery_bar = nullptr;                         ///< 电池进度条
    QLabel *m_label_power = nullptr;                               ///< 电池百分比
    ads::CDockAreaWidget *m_center_docker_area = nullptr;
    std::map<std::string, RatioLayoutedFrame *> m_image_frame_map;
};

#endif // MAPWIDGET_H
