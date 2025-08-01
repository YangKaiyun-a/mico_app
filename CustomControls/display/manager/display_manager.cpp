// 1、图元坐标系 scenPose 对应所有图层的外部全局坐标系
// 2、图层坐标系 每个图层的单独坐标系
// 3、占栅格地图坐标系 occPose
// 4、机器人全局地图坐标系 wordPose

#include "display/manager/display_manager.h"
#include <Eigen/Eigen>
#include <QOpenGLWidget>
#include <boost/filesystem.hpp>
#include <fstream>
#include "algorithm.h"
#include "display/manager/scene_manager.h"
#include "logger.h"
#include "define.h"
#include "signalmanager.h"


namespace Display {

DisplayManager::DisplayManager()
{
    InitUi();
}

DisplayManager::~DisplayManager()
{

}

void DisplayManager::InitUi()
{
    // 用于展示地图等数据的视图窗口
    graphics_view_ptr_ = new ViewManager();
    graphics_view_ptr_->SetDisplayManagerPtr(this);

    // 管理和存储地图等数据和逻辑
    scene_manager_ptr_ = new SceneManager();
    scene_manager_ptr_->Init(graphics_view_ptr_, this);

    // 设置绘制区域
    FactoryDisplay::Instance()->Init(graphics_view_ptr_, scene_manager_ptr_);

    // 会在基类构造函数中调用 FactoryDisplay::Instance()->AddDisplay 将下列派生类添加到 Scene 中
    // 绘制地图
    new DisplayOccMap(DISPLAY_MAP, 1);
    // 绘制全局代价地图
    new DisplayCostMap(DISPLAY_GLOBAL_COST_MAP, 2, DISPLAY_MAP);
    // 绘制局部代价地图
    new DisplayCostMap(DISPLAY_LOCAL_COST_MAP, 3, DISPLAY_MAP);
    // 绘制中心点机器人图标（默认为箭头）
    (new PointShape(PointShape::ePointType::kRobot, DISPLAY_ROBOT, DISPLAY_ROBOT, 9, DISPLAY_MAP))->SetRotateEnable(true);
    // 绘制雷达点
    new LaserPoints(DISPLAY_LASER, 2, DISPLAY_MAP);
    // 绘制全局路径
    new DisplayPath(DISPLAY_GLOBAL_PATH, 6, DISPLAY_MAP);
    // 绘制局部路径
    new DisplayPath(DISPLAY_LOCAL_PATH, 6, DISPLAY_MAP);
    // 绘制机器人形状（默认为正方形）
    new RobotShape(DISPLAY_SHAPE, 8, DISPLAY_MAP);

    SetDisplayConfig(DISPLAY_GLOBAL_PATH + "/Color", Color(0, 0, 255));
    SetDisplayConfig(DISPLAY_LOCAL_PATH + "/Color", Color(0, 255, 0));

    // 当重定位改变时，坐标页面随之移动
    connect(GetDisplay(DISPLAY_ROBOT), SIGNAL(signalPoseUpdate(const RobotPose &)), this, SLOT(slotRobotScenePoseChanged(const RobotPose &)));

    // 设置默认地图图层响应鼠标事件
    FactoryDisplay::Instance()->SetMoveEnable(DISPLAY_MAP);

    // 重定位页面
    set_reloc_pose_widget_ = new SetPoseWidget(graphics_view_ptr_);
    set_reloc_pose_widget_->hide();

    connect(set_reloc_pose_widget_, &SetPoseWidget::SignalHandleOver, this, [=](const bool &is_submit, const RobotPose &pose) {
        SetRelocMode(false);
        if (is_submit)
        {
            Q_EMIT SigManager->sigPub2DPose(pose);
        }
    });
    connect(SigManager, &SignalManager::sigRobotPoseChanged, this, &DisplayManager::onSigRobotPoseChanged);
}

// 接收到 ros2 消息后更新 item
void DisplayManager::UpdateTopicData(const MsgId &id, const std::any &data)
{
    UpdateDisplay(ToString(id), data);
}

// 更新 item
bool DisplayManager::UpdateDisplay(const std::string &display_type, const std::any &data)
{
    VirtualDisplay *display = GetDisplay(display_type);
    if (!display)
    {
        return false;
    }

    if (display_type == DISPLAY_MAP)
    {
        // 全局静态地图
        display->UpdateDisplay(data);
        GetAnyData(OccupancyMap, data, map_data_);
        // 所有图层更新地图数据
        for (auto [name, display] : FactoryDisplay::Instance()->GetTotalDisplayMap())
        {
            display->UpdateMap(map_data_);
        }
        if (!init_flag_)
        {
            scene_manager_ptr_->LoadTopologyMap();
            init_flag_ = true;
        }

    }
    else if (display_type == DISPLAY_ROBOT)
    {
        // 机器人坐标
        // 重定位时屏蔽位置更新
        if (!is_reloc_mode_)
        {
            GetAnyData(RobotPose, data, robot_pose_);
            UpdateRobotPose(robot_pose_);
        }
    }
    else if (display_type == DISPLAY_LASER)
    {
        // 雷达数据
        LaserScan laser_scan;
        GetAnyData(LaserScan, data, laser_scan)
        // 点坐标转换为图元坐标系下
        laser_scan.data = transLaserPoint(laser_scan.data);
        display->UpdateDisplay(laser_scan);
    }
    else if (display_type == DISPLAY_GLOBAL_PATH || display_type == DISPLAY_LOCAL_PATH)
    {
        // 全局/局部路径
        // 激光坐标转换为地图的图元坐标
        RobotPath path_data;
        RobotPath path_data_trans;
        GetAnyData(RobotPath, data, path_data);
        for (auto one_points : path_data)
        {
            // 转换为图元坐标系
            double x, y;
            map_data_.xy2ScenePose(one_points.x, one_points.y, x, y);
            path_data_trans.push_back(Point(x, y));
        }
        display->UpdateDisplay(path_data_trans);
    }
    else
    {
        display->UpdateDisplay(data);
    }
    return true;
}

// 重定位时更新机器人在世界坐标系下的坐标
void DisplayManager::onSigRobotPoseChanged(const RobotPose &pose)
{
    if(!is_reloc_mode_)
    {
        return;
    }

    qDebug() << "重定位发生改变";

    FactoryDisplay::Instance()->SetMoveEnable(DISPLAY_ROBOT, false);
    UpdateRobotPose(pose);

    QTimer::singleShot(300, this, [=]() {
        FactoryDisplay::Instance()->SetMoveEnable(DISPLAY_ROBOT, true);
    });
}

// 当重定位改变时，坐标页面随之移动
void DisplayManager::slotRobotScenePoseChanged(const RobotPose &pose)
{
    if (is_reloc_mode_)
    {
        QPointF occ_pose = GetDisplay(DISPLAY_MAP)->mapFromScene(QPointF(pose.x, pose.y));
        double x, y;
        map_data_.ScenePose2xy(occ_pose.x(), occ_pose.y(), x, y);

        // 更新坐标
        robot_pose_.x = x;
        robot_pose_.y = y;
        robot_pose_.theta = pose.theta;
        QPointF view_pos = graphics_view_ptr_->mapFromScene(QPointF(pose.x, pose.y));
        set_reloc_pose_widget_->move(QPoint(view_pos.x(), view_pos.y()));
        set_reloc_pose_widget_->SetPose(RobotPose(robot_pose_.x, robot_pose_.y, robot_pose_.theta));
    }
}

bool DisplayManager::SetDisplayConfig(const std::string &config_name, const std::any &data)
{
    QString q_config_name = QString::fromStdString(config_name);
    auto config_list = q_config_name.split("/");
    if (config_list.empty() || config_list.size() != 2)
    {
        return false;
    }

    VirtualDisplay *display = GetDisplay(config_list[0].toStdString());
    if (!display)
    {
        std::cout << "error current display not fi csxnd:"
                  << config_list[0].toStdString() << " config_name:" << config_name << std::endl;
        return false;
    }

    // 设置图层是否响应鼠标事件
    if (config_list[1] == "MouseEvent")
    {
        bool is_response;
        GetAnyData(bool, data, is_response);
        display->SetMoveEnable(is_response);
        std::cout << "config:" << config_name << " res:" << is_response << std::endl;
    }
    return display->SetDisplayConfig(config_list[1].toStdString(), data);
}

/**
 * @description:坐标系转换为图元坐标系
 * @return {*}
 */
std::vector<Point> DisplayManager::transLaserPoint(const std::vector<Point> &point)
{
    // point为车身坐标系下的坐标 需要根据当前机器人坐标转换为map
    std::vector<Point> res;
    for (auto one_point : point)
    {
        //根据机器人坐标转换为map坐标系下
        basic::RobotPose map_pose = basic::absoluteSum(
            basic::RobotPose(robot_pose_.x, robot_pose_.y, robot_pose_.theta),
            basic::RobotPose(one_point.x, one_point.y, 0));

        // 转换为图元坐标系
        double x, y;
        map_data_.xy2ScenePose(map_pose.x, map_pose.y, x, y);
        res.push_back(Point(x, y));
    }
    return res;
}

// 更新机器人在世界坐标系下的坐标
void DisplayManager::UpdateRobotPose(const RobotPose &pose)
{
    robot_pose_ = pose;
    GetDisplay(DISPLAY_ROBOT)->UpdateDisplay(wordPose2Map(pose));
    GetDisplay(DISPLAY_SHAPE)->UpdateDisplay(wordPose2Map(pose));
}

void DisplayManager::updateScaled(double value)
{
    FactoryDisplay::Instance()->SetDisplayScaled(DISPLAY_LASER, value);
}

void DisplayManager::SetRelocMode(bool is_start)
{
    is_reloc_mode_ = is_start;

    if (is_start)
    {
        FocusDisplay("");
        set_reloc_pose_widget_->SetPose(RobotPose(robot_pose_.x, robot_pose_.y, robot_pose_.theta));

        // current_scene 为 DISPLAY_ROBOT 图层在大场景 Scene 中的左上角坐标
        auto current_scene = GetDisplay(DISPLAY_ROBOT)->scenePos();

        // 将 current_scene 转换为视图坐标
        QPointF view_pos = graphics_view_ptr_->mapFromScene(current_scene);
        set_reloc_pose_widget_->move(QPoint(view_pos.x(), view_pos.y()));
        set_reloc_pose_widget_->show();
    }
    else
    {
        set_reloc_pose_widget_->hide();
    }

    FactoryDisplay::Instance()->SetMoveEnable(DISPLAY_ROBOT, is_start);
}

void DisplayManager::FocusDisplay(const std::string &display_name)
{
    FactoryDisplay::Instance()->SetFocusDisplay(display_name);
}

/**
 * @description: 世界坐标系点转为全局scene坐标
 * @param {Vector2f&} point 传入的点坐标
 * @return {*}
 */
RobotPose DisplayManager::wordPose2Scene(const RobotPose &point)
{
    // xy在栅格地图上的图元坐标
    double x, y;
    map_data_.xy2ScenePose(point.x, point.y, x, y);

    // xy在map图层上的坐标
    QPointF pose = FactoryDisplay::Instance()->GetDisplay(DISPLAY_MAP)->PoseToScene(QPointF(x, y));
    RobotPose res;
    res.x = pose.x();
    res.y = pose.y();
    res.theta = point.theta;
    return res;
}
/**
 * @description: 世界坐标系点转为以map图层为基准坐标的图元坐标
 * @param {Vector2f&} point 传入的点坐标
 * @return {*}
 */
QPointF DisplayManager::wordPose2Scene(const QPointF &point)
{
    // xy在栅格地图上的图元坐标
    double x, y;
    map_data_.xy2ScenePose(point.x(), point.y(), x, y);
    return FactoryDisplay::Instance()->GetDisplay(DISPLAY_MAP)->PoseToScene(QPointF(x, y));
}

RobotPose DisplayManager::wordPose2Map(const RobotPose &pose)
{
    RobotPose ret = pose;
    double x, y;
    map_data_.xy2ScenePose(pose.x, pose.y, x, y);
    ret.x = x;
    ret.y = y;
    return ret;
}

QPointF DisplayManager::wordPose2Map(const QPointF &pose)
{
    QPointF ret;
    double x, y;
    map_data_.xy2ScenePose(pose.x(), pose.y(), x, y);
    ret.setX(x);
    ret.setY(y);
    return ret;
}

RobotPose DisplayManager::mapPose2Word(const RobotPose &pose)
{
    RobotPose ret = pose;
    double x, y;
    map_data_.ScenePose2xy(pose.x, pose.y, x, y);
    ret.x = x;
    ret.y = y;
    return ret;
}

RobotPose DisplayManager::scenePoseToWord(const RobotPose &pose)
{
    QPointF pose_map = FactoryDisplay::Instance()->GetDisplay(DISPLAY_MAP)->mapFromScene(QPointF(pose.x, pose.y));
    return mapPose2Word(RobotPose(pose_map.x(), pose_map.y(), pose.theta));
}

RobotPose DisplayManager::scenePoseToMap(const RobotPose &pose)
{
    QPointF pose_map = FactoryDisplay::Instance()->GetDisplay(DISPLAY_MAP)->mapFromScene(QPointF(pose.x, pose.y));
    return RobotPose(pose_map.x(), pose_map.y(), pose.theta);
}

// 获取图层
VirtualDisplay *DisplayManager::GetDisplay(const std::string &name)
{
    return FactoryDisplay::Instance()->GetDisplay(name);
}

// 重定位按钮槽函数
void DisplayManager::StartReloc()
{
    if (!set_reloc_pose_widget_->isVisible())
    {
        SetRelocMode(true);
    }
}

void DisplayManager::SetEditMapMode(MapEditMode mode)
{
    scene_manager_ptr_->SetEditMapMode(mode);
}

void DisplayManager::AddOneNavPoint()
{
    scene_manager_ptr_->AddOneNavPoint();
}

OccupancyMap &DisplayManager::GetMap()
{
    return map_data_;
}

void DisplayManager::UpdateMap(OccupancyMap &)
{
    emit signalPubMap(map_data_);
}

void DisplayManager::SaveMap(const std::string &save_path)
{
    LOG_INFO("start save topology map")
    scene_manager_ptr_->SaveTopologyMap(save_path);
    LOG_INFO("start save occ map")
    auto display_map_ = static_cast<DisplayOccMap *>(FactoryDisplay::Instance()->GetDisplay(DISPLAY_MAP));
    auto map = display_map_->GetOccupancyMap();
    map.Save(save_path);
}

void DisplayManager::OpenMap(const std::string &path)
{
    boost::filesystem::path filepath(path);
    //文件夹
    std::string directory = filepath.parent_path().string();
    LOG_INFO("Directory: " << directory);

    // 获取文件名（不包括后缀）
    std::string filenameWithoutExtension = filepath.stem().string();
    LOG_INFO("Filename without extension: " << filenameWithoutExtension);
    // 获取后缀名
    std::string extension = filepath.extension().string();
    LOG_INFO("Extension: " << extension);

    if (extension == ".topology")
    {
        scene_manager_ptr_->OpenTopologyMap(path);
    }
    else if (extension == ".yaml")
    {
        std::string topology_path = directory + "/" + filenameWithoutExtension + ".topology";
        std::string pgm_path = directory + "/" + filenameWithoutExtension + ".pgm";
        std::string yaml_path = directory + "/" + filenameWithoutExtension + ".yaml";

        if (boost::filesystem::exists(topology_path))
        {
            scene_manager_ptr_->OpenTopologyMap(topology_path);
        }

        if (boost::filesystem::exists(yaml_path))
        {
            OccupancyMap map;
            bool ret = map.Load(yaml_path);
            LOG_INFO("open map ret:" << ret);
            UpdateDisplay(DISPLAY_MAP, map);
        }
        else
        {
            LOG_ERROR("pgm or yaml not exit! path:" << directory + "/" + filenameWithoutExtension)
        }
    }
}

void DisplayManager::SetScaleBig()
{
    FactoryDisplay::Instance()->GetDisplay(DISPLAY_MAP)->SetScaled(FactoryDisplay::Instance()->GetDisplay(DISPLAY_MAP)->GetScaleValue() * 1.3);
}

void DisplayManager::SetScaleSmall()
{
    FactoryDisplay::Instance()->GetDisplay(DISPLAY_MAP)->SetScaled(FactoryDisplay::Instance()->GetDisplay(DISPLAY_MAP)->GetScaleValue() * 0.7);
}

}  // namespace Display
