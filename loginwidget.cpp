#include "loginwidget.h"
#include "ui_loginwidget.h"
#include "signalmanager.h"

LoginWidget::LoginWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginWidget)
{
    ui->setupUi(this);
    init();
}

LoginWidget::~LoginWidget()
{
    delete ui;
}

void LoginWidget::init()
{
    ui->label_3->setObjectName("labBold");
    ui->label->setObjectName("labBold");
}

void LoginWidget::on_LoginBtn_clicked()
{
    // TODO: 密码验证，并记录到操作日志


    Q_EMIT SigManager->sigLogin(true);
}

