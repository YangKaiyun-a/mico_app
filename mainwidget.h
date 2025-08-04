#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

namespace Ui {
class MainWidget;
}

enum FUNCTION_INDEX
{
    HOME,
    TASK,
    MAP,
    RECORD,
    ACCOUNT,
    DEBUG
};

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();
    void init();
    void initStackWidget();

private:
    Ui::MainWidget *ui;
};

#endif // MAINWIDGET_H
