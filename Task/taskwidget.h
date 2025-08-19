#ifndef TASKWIDGET_H
#define TASKWIDGET_H

#include "tasklistwidget.h"
#include "taskfinishedwidget.h"

#include <QWidget>


namespace Ui {
class TaskWidget;
}

class TaskWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TaskWidget(QWidget *parent = nullptr);
    ~TaskWidget();
    void init();
    void initData();
    void initUI();
    void enterLogBefore();                                  ///< 进入该页面前的准备事务

private:
    Ui::TaskWidget *ui;
    TaskListWidget *m_taskListWidget = nullptr;
    taskFinishedWidget *m_taskFinishedWidget = nullptr;
};

#endif // TASKWIDGET_H
