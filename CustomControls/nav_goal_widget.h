#pragma once
#include <QCalendarWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QGraphicsItem>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QtWidgets/QListWidget>
#include "algorithm.h"
#include "point_type.h"
#include "joystick.h"

using namespace basic;

class NavGoalWidget : public QWidget {
    Q_OBJECT
public:
    enum HandleResult
    {
        kSend = 0,
        kRemove = 1,
        kCancel = 2
    };

    struct PointInfo
    {
        RobotPose pose;
        QString name;
    };

private:
    QDoubleSpinBox *spinBox_x_;
    QDoubleSpinBox *spinBox_y_;
    QDoubleSpinBox *spinBox_theta_;
    QLineEdit *lineEdit_name_;

signals:
    void SignalHandleOver(const HandleResult &flag, const RobotPose &pose);

public slots:
    void SetPose(const PointInfo &pose);
    void SetEditEnabled(bool flag);

private slots:
    void onValueChanged(double);

public:
    NavGoalWidget(QWidget *parent = 0);
    ~NavGoalWidget() {}
};
