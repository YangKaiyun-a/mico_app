#ifndef HOMEWIDGET_H
#define HOMEWIDGET_H
#include "statusmonitorwidget.h"
#include "currenttaskwidget.h"
#include "global.h"

#include <QWidget>
#include <QMap>

namespace Ui {
class HomeWidget;
}

class HomeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HomeWidget(QWidget *parent = nullptr);
    ~HomeWidget();
    void init();
    void initData();
    void initUI();
    void initTaskWidget();
    void initRobotWidget();
    void initDeviceWidget();

private:
    Ui::HomeWidget *ui;
    CurrentTaskWidget *m_currentTaskWidget = nullptr;
    QMap<ENUM_CLASS::TASK_STATUS, StatusMonitorWidget*> m_taskMonitorMap;
};

#endif // HOMEWIDGET_H
