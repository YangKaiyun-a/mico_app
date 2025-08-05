#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

namespace Ui {
class MainWidget;
}



class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();
    void init();
    void initStackWidget();

private slots:
    void on_btnFirst_clicked();

    void on_btnTaskList_clicked();

    void on_btnMap_clicked();

    void on_btnLogger_clicked();

    void on_btnUsers_clicked();

    void on_btnDebug_clicked();

private:
    Ui::MainWidget *ui;
};

#endif // MAINWIDGET_H
