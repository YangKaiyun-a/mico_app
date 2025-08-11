#pragma once
#include <mutex>
#include "topology_map.h"
#include "global.h"

#include <QMap>

class ConfigManager {

public:
    ~ConfigManager();
    bool StoreConfig();
    void Init(const std::string &config_path);
    static bool writeStringToFile(const std::string &filePath, const std::string &content);         ///< 写配置文件
    static ConfigManager *Instacnce();
    void SetDefaultConfig(const std::string &name, const std::string &value);
    ConfigRoot &GetRootConfig() { return config_root_; }
    bool ReadTopologyMap(const std::string &map_path, TopologyMap &map);                            ///< 读取地图
    bool WriteTopologyMap(const std::string &map_path, const TopologyMap &topology_map);            ///< 保存地图

    /*************************/
    bool updateRobotShapedConfig(int robotId, const QString &config);           ///< 更新机器人形状
    RobotShapedConfig getRobotShapedConfig(int robotId);                        ///< 获取机器人形状


private:
    ConfigManager();
    // 禁用拷贝构造函数和赋值运算符
    ConfigManager(const ConfigManager &) = delete;
    ConfigManager &operator=(const ConfigManager &) = delete;
    bool ReadRootConfig();                                          ///< 解析配置文件到 config_root_

private:
    std::string config_path_;       ///< 配置文件路径
    ConfigRoot config_root_;        ///< json 根节点
    std::mutex mutex_;


    /**************************/
    static ConfigManager* m_configManager;
    QMap<int, RobotShapedConfig> m_robotShapedConfigMap;        ///< 根据 id 存储机器人形状

};

#define CfgManager ConfigManager::Instacnce()
