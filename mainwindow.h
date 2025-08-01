#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
#include "manager/channel_manager.h"
#include "config_manager.h"
#include "display/manager/display_manager.h"
#include "point_type.h"
#include "dashboard.h"
#include "nav_goal_table_view.h"
#include "set_pose_widget.h"
#include "speed_ctrl.h"
#include "ratio_layouted_frame.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void SendChannelMsg(const MsgId &id, const std::any &data);             ///< 发送 ROS2 数据

public slots:
    void onSigCursorPose(QPointF pos);
    void onSigSendNavGoal(const std::any &data);           ///< 发送目标点位的槽函数
    void onSigPub2DPose(const basic::RobotPose &pose);                      ///< 发送位姿的槽函数
    void onSigPub2DGoal(const basic::RobotPose &pose);                      ///< 发送目标点位的槽函数
    void onSigRecvChannelData(const MsgId &id, const std::any &data);       ///< 对接收到的 ROS2 数据进行分发
    void updateOdomInfo(RobotState state);
    void RestoreState();
    void SlotSetBatteryStatus(double percent, double voltage);
    void SlotRecvImage(const std::string &location, std::shared_ptr<cv::Mat> data);

protected:
    virtual void closeEvent(QCloseEvent *event) override;


private:
    void init();
    void initData();
    void initUI();
    void closeChannel();
    void SaveState();

private:
    QAction *SavePerspectiveAction = nullptr;
    QWidgetAction *PerspectiveListAction = nullptr;
    Ui::MainWindow *ui;
    DashBoard *speed_dash_board_;                       ///< 速度仪表盘
    ads::CDockManager *dock_manager_;
    ads::CDockAreaWidget *StatusDockArea;
    ads::CDockWidget *TimelineDockWidget;
    Display::DisplayManager *display_manager_;          ///< 管理地图显示
    QLabel *label_pos_map_;
    QLabel *label_pos_scene_;
    QThread message_thread_;
    SpeedCtrlWidget *speed_ctrl_widget_;                ///< 速度控制页面
    NavGoalTableView *nav_goal_table_view_;             ///< 任务列表页面
    QProgressBar *battery_bar_;                         ///< 电池进度条
    QLabel *label_power_;                               ///< 电池百分比
    ads::CDockAreaWidget *center_docker_area_;
    std::map<std::string, RatioLayoutedFrame *> image_frame_map_;
};
#endif  // MAINWINDOW_H
