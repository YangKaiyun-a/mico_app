#ifndef LOGGERWIDGET_H
#define LOGGERWIDGET_H

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

private:
    Ui::LoggerWidget *ui;
};

#endif // LOGGERWIDGET_H
