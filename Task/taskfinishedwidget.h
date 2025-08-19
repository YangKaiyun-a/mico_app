#ifndef TASKFINISHEDWIDGET_H
#define TASKFINISHEDWIDGET_H

#include <QWidget>
#include <QSqlTableModel>

namespace Ui {
class TaskFinishedWidget;
}

class TaskFinishedWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TaskFinishedWidget(QWidget *parent = nullptr);
    ~TaskFinishedWidget();
    void init();
    void initData();
    void initUI();
    void enterLogBefore();                                  ///< 进入该页面前的准备事务
    void updateTableModel(const QString &strFilter);        ///< 更新数据模型
    void updateTableWidget(int beginRow, int endRow);       ///< 更新数据视图

private:
    Ui::TaskFinishedWidget *ui;
    QSqlTableModel *m_tableModel = nullptr;   ///< 最底层的 DataBaseTable::m_tableModel
};

#endif // TASKFINISHEDWIDGET_H
