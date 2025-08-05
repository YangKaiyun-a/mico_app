#ifndef ROBOTSTATUSWIDGET_H
#define ROBOTSTATUSWIDGET_H

#include <QWidget>

namespace Ui {
class RobotStatusWidget;
}

class RobotStatusWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RobotStatusWidget(QWidget *parent = nullptr);
    ~RobotStatusWidget();

private:
    Ui::RobotStatusWidget *ui;
};

#endif // ROBOTSTATUSWIDGET_H
