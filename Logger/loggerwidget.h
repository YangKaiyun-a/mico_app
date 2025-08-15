#ifndef LOGGERWIDGET_H
#define LOGGERWIDGET_H

#include "operationlogwidget.h"

#include <QWidget>

namespace Ui {
class LoggerWidget;
}

class LoggerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoggerWidget(QWidget *parent = nullptr);
    ~LoggerWidget();
    void init();
    void initData();
    void initUI();
    void enterLogBefore();          ///< 进入该页面前的准备事务

private slots:
    void on_btnOperationLog_clicked();

    void on_btnSystemLog_clicked();

private:
    Ui::LoggerWidget *ui;
    OperationLogWidget *m_operationWidget = nullptr;
};

#endif // LOGGERWIDGET_H
