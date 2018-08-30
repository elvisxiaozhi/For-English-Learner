#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "chesslbl.h"
#include "resultwidget.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    static int crossWinningTimes, circleWinningTimes;

private:
    Ui::Widget *ui;
    ChessLbl **lblArr[3];
    bool isXTurn;
    ResultWidget *result;

    void setWidgetLayout();
    void setLbl();
    int checkWin();
    bool isWinning(int);

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    void toolBtnClicked(bool);
    void lblClicked(int, int);
    void restartGame();
    void modeChanged(int);
};

#endif // WIDGET_H
