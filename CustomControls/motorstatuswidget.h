#ifndef MOTORSTATUSWIDGET_H
#define MOTORSTATUSWIDGET_H

#include <QWidget>

namespace Ui {
class MotorStatusWidget;
}

class MotorStatusWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MotorStatusWidget(QWidget *parent = nullptr);
    ~MotorStatusWidget();

private:
    Ui::MotorStatusWidget *ui;
};

#endif // MOTORSTATUSWIDGET_H
