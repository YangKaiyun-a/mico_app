#include "operatebtnwidget.h"
#include "ui_operatebtnwidget.h"

#include <QMouseEvent>

OperateBtnWidget::OperateBtnWidget(const QString &workflowName, QWidget *parent)
    : QWidget(parent), m_workflowName(workflowName)
    , ui(new Ui::OperateBtnWidget)
{
    ui->setupUi(this);
}

OperateBtnWidget::~OperateBtnWidget()
{
    delete ui;
}

void OperateBtnWidget::init()
{
    // 安装过滤器，将鼠标点击事件交给父对象处理
    ui->btnStart->installEventFilter(this);
    ui->btnStop->installEventFilter(this);
}

bool OperateBtnWidget::eventFilter(QObject *watch, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *tMouseEvent = (QMouseEvent *)event;
        if (tMouseEvent->button() == Qt::LeftButton)
        {
            QPushButton *Btn = (QPushButton *)watch;
            if (nullptr != Btn)
            {
                if (ui->btnStart == Btn)
                {

                    m_currentBtnType = ENUM_CLASS::TASK_BUTTON_TYPE::START;
                }
                else
                {
                    m_currentBtnType = ENUM_CLASS::TASK_BUTTON_TYPE::STOP;
                }
                return QWidget::event(event);
            }
        }
    }

    return false;
}

QString OperateBtnWidget::workflowName() const
{
    return m_workflowName;
}

ENUM_CLASS::TASK_BUTTON_TYPE OperateBtnWidget::currentBtnType() const
{
    return m_currentBtnType;
}
