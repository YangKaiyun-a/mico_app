#ifndef TASKWIDGET_H
#define TASKWIDGET_H

#include <QWidget>
#include <QSqlTableModel>

namespace Ui {
class TaskWidget;
}

class TaskWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TaskWidget(QWidget *parent = nullptr);
    ~TaskWidget();
    void init();
    void initData();
    void initUI();
    void enterLogBefore();                                  ///< 进入该页面前的准备事务
    void updateTableModel(const QString &strFilter);        ///< 更新数据模型
    void updateTableWidget(int beginRow, int endRow);       ///< 更新数据视图

private:
    Ui::TaskWidget *ui;
    QSqlTableModel *m_tableModel = nullptr;   ///< 最底层的 DataBaseTable::m_tableModel
};

#endif // TASKWIDGET_H
