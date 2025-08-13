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

private:
    Ui::LoggerWidget *ui;
    OperationLogWidget *m_operationWidget = nullptr;
};

#endif // LOGGERWIDGET_H
