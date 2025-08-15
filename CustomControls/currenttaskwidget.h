/*当前任务监控窗口*/



#ifndef CURRENTTASKWIDGET_H
#define CURRENTTASKWIDGET_H

#include <QWidget>

namespace Ui {
class CurrentTaskWidget;
}

class CurrentTaskWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CurrentTaskWidget(const QString& title, const QString& taskName, QWidget *parent = nullptr);
    ~CurrentTaskWidget();
    void setTite(const QString &title);
    void setContent(const QString &content);
    void seTaskName(const QString &taskName);

private:
    Ui::CurrentTaskWidget *ui;
    QString m_title;
    QString m_content;
    QString m_taskName;
};

#endif // CURRENTTASKWIDGET_H
