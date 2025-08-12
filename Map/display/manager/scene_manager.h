#pragma once
#include "../virtual_display.h"
#include <QCursor>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include "topology_map.h"
#include "../point_shape.h"
#include "../../nav_goal_widget.h"
#include "../../set_pose_widget.h"
namespace Display {

class DisplayManager;
class SceneManager : public QGraphicsScene {
    Q_OBJECT
private:
    SetPoseWidget *set_nav_pose_widget_;
    QGraphicsView *view_ptr_;
    Display::VirtualDisplay *curr_handle_display_{nullptr};
    DisplayManager *display_manager_;
    NavGoalWidget *nav_goal_widget_;    ///< 设置目标点位页面

    TopologyMap topology_map_;          ///< 存储地图数据
    MapEditMode current_mode_;
    bool right_pressed_{false};
    bool left_pressed_{false};
    double eraser_range_{3};
    QCursor eraser_cursor_;
    QCursor move_cursor_;
    QCursor line_cursor_;
    QCursor nav_goal_cursor_;
    QCursor rect_cursor_;
    QCursor region_cursor_;
    QCursor pen_cursor_;

public slots:
    void SetEditMapMode(MapEditMode mode);


public:
    SceneManager(QObject *parent = nullptr);
    virtual ~SceneManager();
    void Init(QGraphicsView *view_ptr, DisplayManager *manager);
    void AddOneNavPoint();
    void LoadTopologyMap();                                             ///< 启动软件时加载配置文件的点位数据（暂未启用）
    void OpenTopologyMap(const std::string &file_path);                 ///< 读取点位数据到 topology_map_
    void SaveTopologyMap(const std::string &file_path);                 ///< 保存点位数据
    void updateDefaultTopologyMap();                                    ///< 更改配置文件中的点位数据

private:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void wheelEvent(QGraphicsSceneWheelEvent *event) override;
    void blindNavGoalWidget(Display::VirtualDisplay *);
    void updateNavGoalWidgetPose( Display::VirtualDisplay *, bool is_move = true);
    std::string generatePointName(const std::string &prefix);
    void eraseScenePointRange(const QPointF &, double);
    void setEraseCursor();
    void drawPoint(const QPointF &);
    void SetPointMoveEnable(bool is_enable);
};
}  // namespace Display
