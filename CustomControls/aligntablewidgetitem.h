/*设置 QTaleWidget 列表内容显示居中*/

#ifndef ALIGNTABLEWIDGETITEM_H
#define ALIGNTABLEWIDGETITEM_H

#include <QTableWidgetItem>

class AlignTableWidgetItem : public QTableWidgetItem
{
public:
    explicit AlignTableWidgetItem(QString text = "", int type = Type);
};

#endif // ALIGNTABLEWIDGETITEM_H
