#ifndef TASKFINISHEDWIDGET_H
#define TASKFINISHEDWIDGET_H

#include <QWidget>

namespace Ui {
class taskFinishedWidget;
}

class taskFinishedWidget : public QWidget
{
    Q_OBJECT

public:
    explicit taskFinishedWidget(QWidget *parent = nullptr);
    ~taskFinishedWidget();

private:
    Ui::taskFinishedWidget *ui;
};

#endif // TASKFINISHEDWIDGET_H
