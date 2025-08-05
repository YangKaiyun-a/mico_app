/*
 * 用于显示任务的执行情况
 * 待执行、执行中、已完成、失败
 */


#ifndef STATUSMONITORWIDGET_H
#define STATUSMONITORWIDGET_H

#include <QWidget>

namespace Ui {
class StatusMonitorWidget;
}

class StatusMonitorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StatusMonitorWidget(const QString& title, QWidget *parent = nullptr);
    ~StatusMonitorWidget();
    void setTite(const QString &title);
    void setContent(const QString &content);

private:
    Ui::StatusMonitorWidget *ui;
    QString m_title;
    QString m_content;
};

#endif // STATUSMONITORWIDGET_H
