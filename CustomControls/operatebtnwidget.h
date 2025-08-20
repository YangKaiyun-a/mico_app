#ifndef OPERATEBTNWIDGET_H
#define OPERATEBTNWIDGET_H

#include "global.h"

#include <QWidget>

namespace Ui {
class OperateBtnWidget;
}

class OperateBtnWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OperateBtnWidget(const QString &workflowName, QWidget *parent = nullptr);
    ~OperateBtnWidget();
    void init();

    ENUM_CLASS::TASK_BUTTON_TYPE currentBtnType() const;

    QString workflowName() const;

protected:
    bool eventFilter(QObject *watch, QEvent *event) override;

private:
    Ui::OperateBtnWidget *ui;
    ENUM_CLASS::TASK_BUTTON_TYPE m_currentBtnType;                   ///< 当前按钮状态
    QString m_workflowName = "";
};

#endif // OPERATEBTNWIDGET_H
