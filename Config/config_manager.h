#pragma once
#include <mutex>
#include "config_define.h"
#include "topology_map.h"

#ifndef GET_TOPIC_NAME
#define GET_TOPIC_NAME(frame_name) \
Config::ConfigManager::Instacnce()->GetTopicName(frame_name)
#endif

#ifndef SET_DEFAULT_TOPIC_NAME
#define SET_DEFAULT_TOPIC_NAME(frame_name, topic_name)                \
    Config::ConfigManager::Instacnce()->SetDefaultTopicName(frame_name, \
                              topic_name);
#endif

namespace Config {

class ConfigManager {

public:
    ~ConfigManager();
    bool StoreConfig();
    void Init(const std::string &config_path);
    static bool writeStringToFile(const std::string &filePath, const std::string &content);         ///< 写配置文件
    static ConfigManager *Instacnce();
    std::string GetTopicName(const std::string &display_name);                                      ///< 根据 display_name 获取 topic，失败则返回空
    void SetDefaultConfig(const std::string &name, const std::string &value);
    void SetDefaultTopicName(const std::string &display_name, const std::string &topic_name);       ///< 为 display_name 设置默认的 topic
    ConfigRoot &GetRootConfig() { return config_root_; }
    bool ReadTopologyMap(const std::string &map_path, TopologyMap &map);                            ///< 读取地图
    bool WriteTopologyMap(const std::string &map_path, const TopologyMap &topology_map);            ///< 保存地图

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





};
}  // namespace Config
