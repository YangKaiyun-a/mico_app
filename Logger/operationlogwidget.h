#ifndef OPERATIONLOGWIDGET_H
#define OPERATIONLOGWIDGET_H

#include <QWidget>
#include <QSqlTableModel>

namespace Ui {
class OperationLogWidget;
}

class OperationLogWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OperationLogWidget(QWidget *parent = nullptr);
    ~OperationLogWidget();
    void init();
    void initData();
    void initUI();

private:
    Ui::OperationLogWidget *ui;
    QSqlTableModel *m_tabModel = nullptr;   ///< 最底层的 DataBaseTable::m_tableModel
};

#endif // OPERATIONLOGWIDGET_H
