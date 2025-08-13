#ifndef OPERATIONLOGWIDGET_H
#define OPERATIONLOGWIDGET_H

#include <QWidget>

namespace Ui {
class OperationLogWidget;
}

class OperationLogWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OperationLogWidget(QWidget *parent = nullptr);
    ~OperationLogWidget();

private:
    Ui::OperationLogWidget *ui;
};

#endif // OPERATIONLOGWIDGET_H
