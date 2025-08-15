#include "aligntablewidgetitem.h"


AlignTableWidgetItem::AlignTableWidgetItem(QString text, int type) : QTableWidgetItem(text, type)
{
    setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
}
