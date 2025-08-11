#include "settingfilecreate.h"

#include <boost/dll.hpp>
#include <fstream>
#include <QSettings>
#include <QDir>
#include <QDebug>

SettingFileCreate* SettingFileCreate::m_settingFileCreate = nullptr;

SettingFileCreate::SettingFileCreate(QObject *parent)
    : QObject{parent}
{
    init();
}

SettingFileCreate *SettingFileCreate::instance()
{
    if(!m_settingFileCreate)
    {
        m_settingFileCreate = new SettingFileCreate();
    }

    return m_settingFileCreate;
}

void SettingFileCreate::init()
{
    //创建配置文件
    CreateSettingFile();

    //判断文件是否存在
    if(!QFile::exists(m_configFile))
    {
        //不存在则创建一个新文件并打开
        QFile load_file(m_configFile);
        //检查文件的打开和存在性
        if (Q_UNLIKELY(!load_file.open(QIODevice::ReadOnly | QIODevice::Append) || !load_file.exists()))
        {
            qWarning() << "--------配置文件创建失败--------";
            return;
        }
        load_file.close();

        //参数初始化
        SettingInit();
    }

}

//创建配置文件
void SettingFileCreate::CreateSettingFile()
{
    //生成config目录
    m_configDir.setPath("config");

    //创建config文件夹（如果不存在）
    if(!m_configDir.exists())
    {
        m_configDir.mkpath(".");
    }

    //配置文件路径
    m_configFile = m_configDir.absoluteFilePath("config.ini");

    //指定INI文件路径
    if(!m_settings)
    {
        m_settings = new QSettings(m_configFile, QSettings::IniFormat, this);
    }
}

//参数初始化
void SettingFileCreate::SettingInit()
{
    m_settings->beginGroup("database");
    m_settings->setValue("db_connected", false);
    m_settings->setValue("db_Host", "192.168.137.101");
    m_settings->setValue("db_Name", "postgres");
    m_settings->setValue("db_Port", "5432");
    m_settings->setValue("db_User", "postgres");
    m_settings->setValue("db_PWD", "12345678");
    m_settings->endGroup();
}

QVariant SettingFileCreate::sysValue(const QString &group, const QString &key, const QVariant &defaultValue)
{
    if(!m_settings)
    {
        return QVariant();
    }

    QVariant value;

    m_settings->beginGroup(group);
    value = m_settings->value(key, defaultValue);
    m_settings->endGroup();

    return value;
}

bool SettingFileCreate::WriteTopologyMap(const QString &map_path, const TopologyMap &topology_map)
{
    return true;
}

bool SettingFileCreate::ReadTopologyMap(const QString &map_path, TopologyMap &map)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    std::string pathStd = map_path.toStdString();
    std::string fullPath = pathStd;

    boost::filesystem::path inputPathObj(pathStd);
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

    if (parseContext.parseTo(map) != JS::Error::NoError)
    {
        std::string errorStr = parseContext.makeErrorString();
        fprintf(stderr, "Error parsing struct %s\n", errorStr.c_str());
        return false;
    }
    return true;
}
