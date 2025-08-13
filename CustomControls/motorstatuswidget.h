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
    explicit MotorStatusWidget(const QString &labName, const QString &labStatus, QWidget *parent = nullptr);
    ~MotorStatusWidget();

    void setLabName(const QString &newLabName);

    void setLabStatus(const QString &newLabStatus);

private:
    Ui::MotorStatusWidget *ui;
    QString m_labName;
    QString m_labStatus;
};

#endif // MOTORSTATUSWIDGET_H
