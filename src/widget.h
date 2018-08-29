#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "chesslbl.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;
    QVector<ChessLbl *> lblVec;

    void setWidgetLayout();

private slots:
    void toolBtnClicked(bool);
};

#endif // WIDGET_H
