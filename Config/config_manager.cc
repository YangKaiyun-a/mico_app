#include "config_manager.h"
#include <QFile>
#include <QDebug>
#include <boost/dll.hpp>
#include <fstream>


ConfigManager *ConfigManager::Instacnce()
{
    static ConfigManager instance;
    return &instance;
}

ConfigManager::ConfigManager()
{

}

QMap<int, MotorStatus> ConfigManager::motorStatusMap() const
{
    return m_motorStatusMap;
}

// 电机状态->字符串
QString ConfigManager::motorStatusToString(ENUM_CLASS::MOTOR_STATUS status)
{
    switch(status)
    {
    case ENUM_CLASS::MOTOR_STATUS::NONE:
        return  QStringLiteral("暂未获取");
    case ENUM_CLASS::MOTOR_STATUS::NORMAL:
        return  QStringLiteral("正常");
    case ENUM_CLASS::MOTOR_STATUS::WARNING:
        return  QStringLiteral("警告");
    case ENUM_CLASS::MOTOR_STATUS::ERROR:
        return  QStringLiteral("故障");
    default:
        return  QStringLiteral("获取失败");
    }
}

// 任务状态->字符串
QString ConfigManager::taskStatusToString(int status)
{
    switch(status)
    {
    case ENUM_CLASS::TASK_STATUS::WAITING:
        return  QStringLiteral("待执行");
    case ENUM_CLASS::TASK_STATUS::EXECUTING:
        return  QStringLiteral("执行中");
    case ENUM_CLASS::TASK_STATUS::FINISH:
        return  QStringLiteral("已完成");
    case ENUM_CLASS::TASK_STATUS::FAILED:
        return  QStringLiteral("失败");
    default:
        return  QStringLiteral("获取失败");
    }
}


void ConfigManager::setMotorStatusMap(const QMap<int, MotorStatus> &newMotorStatusMap)
{
    m_motorStatusMap = newMotorStatusMap;
}

ConfigManager::~ConfigManager()
{

}


// 写配置文件
bool ConfigManager::writeStringToFile(const std::string &filePath, const std::string &content)
{
    boost::filesystem::path path(filePath);
    if (!boost::filesystem::exists(path.parent_path()))
    {
        boost::filesystem::create_directories(path.parent_path());  // 创建路径
    }

    std::ofstream outputFile(filePath);
    if (outputFile)
    {
        outputFile << content;  // 写入内容
        outputFile.close();     // 关闭文件
        return true;
    }
    else
    {
        std::cerr << "无法创建文件 " << filePath << std::endl;
        return false;
    }
}

// 读取地图中的点位信息
bool ConfigManager::ReadTopologyMap(const std::string &map_path, TopologyMap &map)
{
    std::lock_guard<std::mutex> lock(mutex_);
    std::string fullPath = map_path;
    boost::filesystem::path inputPathObj(map_path);
    // 判断路径是否为相对路径
    if (inputPathObj.is_relative())
    {
        // 获取当前可执行程序的路径
        boost::filesystem::path executablePath = boost::dll::program_location();

        // 使用 absolute 函数将相对路径转换为绝对路径
        boost::filesystem::path absolutePath = boost::filesystem::absolute(inputPathObj, executablePath.parent_path());

        // 将路径转换为字符串
        fullPath = absolutePath.string();
    }

    std::ifstream file(fullPath);
    std::string json((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    JS::ParseContext parseContext(json);

    // JS::ParseContext has the member
    if (parseContext.parseTo(map) != JS::Error::NoError)
    {
        std::string errorStr = parseContext.makeErrorString();
        fprintf(stderr, "Error parsing struct %s\n", errorStr.c_str());
        return false;
    }
    return true;
}

// 保存地图点位信息到指定路径
bool ConfigManager::WriteTopologyMap(const std::string &map_path, const TopologyMap &topology_map)
{
    std::lock_guard<std::mutex> lock(mutex_);
    std::string fullPath = map_path;
    boost::filesystem::path inputPathObj(map_path);

    // 判断路径是否为相对路径
    if (inputPathObj.is_relative())
    {
        // 获取当前可执行程序的路径
        boost::filesystem::path executablePath = boost::dll::program_location();

        // 使用 absolute 函数将相对路径转换为绝对路径
        boost::filesystem::path absolutePath = boost::filesystem::absolute(inputPathObj, executablePath.parent_path());

        // 将路径转换为字符串
        fullPath = absolutePath.string();
    }
    std::string pretty_json = JS::serializeStruct(topology_map);
    return writeStringToFile(fullPath, pretty_json);
}

// 更新机器人形状
bool ConfigManager::updateRobotShapedConfig(int robotId, const QString &config)
{
    if(config.isEmpty())
    {
        qWarning() << "机器人形状数据为空";
    }

    // QString -> std::string
    std::string json = config.toStdString();
    JS::ParseContext parseContext(json);

    // 解析到临时变量中
    RobotShapedConfig tmpRobotShapedConfig;
    if (parseContext.parseTo(tmpRobotShapedConfig) != JS::Error::NoError)
    {
        std::string errorStr = parseContext.makeErrorString();
        fprintf(stderr, "Error parsing config.json error: %s\n", errorStr.c_str());
        std::exit(1);
    }

    // 添加到 QMap
    auto it = m_robotShapedConfigMap.find(robotId);
    if (it != m_robotShapedConfigMap.end())
    {
        it.value() = std::move(tmpRobotShapedConfig);
    }
    else
    {
        m_robotShapedConfigMap.insert(robotId, std::move(tmpRobotShapedConfig));
    }

    return true;
}

// 获取机器人形状
RobotShapedConfig ConfigManager::getRobotShapedConfig(int robotId)
{
    if(!m_robotShapedConfigMap.contains(robotId))
    {
        qWarning() << robotId << "号机器人不存在，形状获取失败";
        return RobotShapedConfig();
    }
    else
    {
        return m_robotShapedConfigMap.value(robotId);
    }
}
