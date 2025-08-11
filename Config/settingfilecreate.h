#ifndef SETTINGFILECREATE_H
#define SETTINGFILECREATE_H

#include "topology_map.h"

#include <mutex>
#include <QObject>
#include <QDir>
#include <QVariant>
#include <QSettings>

class SettingFileCreate : public QObject
{
    Q_OBJECT

public:
    explicit SettingFileCreate(QObject *parent = nullptr);
    static SettingFileCreate* instance();
    void init();
    void CreateSettingFile();
    void SettingInit();
    QVariant sysValue(const QString& group, const QString& key, const QVariant& defaultValue = QVariant());
    bool ReadTopologyMap(const QString &map_path, TopologyMap &map);                            ///< 读取地图
    bool WriteTopologyMap(const QString &map_path, const TopologyMap &topology_map);            ///< 保存地图


private:
    static SettingFileCreate* m_settingFileCreate;
    QDir m_configDir;                                   ///< config的目录
    QString m_configFile;                               ///< config的文件路径
    QSettings *m_settings = nullptr;
    std::mutex m_mutex;
};

#define settingManager SettingFileCreate::instance()

#endif // SETTINGFILECREATE_H
