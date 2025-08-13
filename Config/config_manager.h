#pragma once
#include <mutex>
#include "topology_map.h"
#include "global.h"

#include <QMap>

class ConfigManager {

public:
    ~ConfigManager();

    static bool writeStringToFile(const std::string &filePath, const std::string &content);         ///< 写配置文件
    static ConfigManager *Instacnce();
    bool ReadTopologyMap(const std::string &map_path, TopologyMap &map);                            ///< 读取地图中的点位信息
    bool WriteTopologyMap(const std::string &map_path, const TopologyMap &topology_map);            ///< 保存地图点位信息到指定路径
    bool updateRobotShapedConfig(int robotId, const QString &config);                               ///< 更新机器人形状
    RobotShapedConfig getRobotShapedConfig(int robotId);                                            ///< 获取机器人形状
    void setMotorStatusMap(const QMap<int, MotorStatus> &newMotorStatusMap);
    QMap<int, MotorStatus> motorStatusMap() const;
    QString motorStatusToString(ENUM_CLASS::MOTOR_STATUS status);                                   ///< 电机状态->字符串

private:
    ConfigManager();
    ConfigManager(const ConfigManager &) = delete;
    ConfigManager &operator=(const ConfigManager &) = delete;

private:
    std::mutex mutex_;
    static ConfigManager* m_configManager;
    QMap<int, RobotShapedConfig> m_robotShapedConfigMap;        ///< 根据 id 存储机器人形状（唯一一份）
    QMap<int, MotorStatus> m_motorStatusMap;                    ///< 根据 id 存储电机状态（唯一一份）

};

#define CfgManager ConfigManager::Instacnce()
