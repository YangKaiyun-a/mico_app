#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>

namespace Ui {
class LoginWidget;
}

class LoginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = nullptr);
    ~LoginWidget();
    void init();

private slots:
    void on_LoginBtn_clicked();

private:
    Ui::LoginWidget *ui;
};

#endif // LOGINWIDGET_H
