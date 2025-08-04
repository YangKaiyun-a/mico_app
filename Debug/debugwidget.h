#ifndef DEBUGWIDGET_H
#define DEBUGWIDGET_H

#include <QWidget>

namespace Ui {
class DebugWidget;
}

class DebugWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DebugWidget(QWidget *parent = nullptr);
    ~DebugWidget();

private:
    Ui::DebugWidget *ui;
};

#endif // DEBUGWIDGET_H
