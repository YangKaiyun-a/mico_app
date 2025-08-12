#ifndef GLOBAL_H
#define GLOBAL_H

#include <map>
#include "json_struct/json_struct.h"

#include <QObject>



/***************************枚举类型******************************/
class ENUM_CLASS : public QObject {
    Q_OBJECT

public:
    explicit ENUM_CLASS(QObject *parent = nullptr);

    // 主页面的功能模块
    enum FUNCTION_INDEX
    {
        HOME,
        TASK,
        MAP,
        RECORD,
        ACCOUNT,
        DEBUG
    };


    // 任务状态
    enum TASK_STATUS
    {
        WAITING,
        EXECUTING,
        FINISH,
        FAILED
    };
};




/***************************结构体******************************/


struct ImageDisplayConfig {
    std::string location;
    std::string topic;
    bool enable = true;

    JS_OBJ(location, topic, enable);
};

struct ShapePoint {
    double x;
    double y;
    JS_OBJ(x, y);
};

// 机器人形状
struct RobotShapedConfig {
    std::vector<ShapePoint> shaped_points;
    bool is_ellipse = false;
    std::string color{"0x00000FF"};
    float opacity{0.5};
    JS_OBJ(shaped_points, is_ellipse, color, opacity);
};





/***************************Topic********************************/

// 导航目标点
#define GOAL_POSE "/goal_pose"

// 机器人重新定位初始位姿（重定位）
#define INITIAL_POSE "/initialpose"

// 全局静态地图数据（通常是栅格地图）
#define GLOBAL_MAP "/map"

// 局部代价地图，反映机器人当前位置周围的障碍物分布
#define LOCAL_COST_MAP "/local_costmap/costmap"

// 全局代价地图，反映整个环境的可通行区域和障碍物分布
#define GLOBAL_COST_MAP "/global_costmap/costmap"

// 激光雷达扫描数据
#define LASER_SCAN "/scan"

// 全局路径，从当前位置到目标点的最优路径
#define GLOBAL_PLAN "/plan"

// 局部路径，结合实时障碍物/动态目标，计算的短时可行路线
#define LOCAL_PLAN "/local_plan"

// 里程计数据，通常包括机器人当前位置、速度、姿态
#define ODOM "/odom"

// 控制机器人运动的目标线速度和角速度
#define SPEED "/cmd_vel"

// 机器人当前电量、电压、健康状况
#define BATTERY "/battery"

#endif // GLOBAL_H
