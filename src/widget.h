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
    bool isXTurn;

    void setWidgetLayout();
    void setLbl();
    void checkWin();
    int checkEightDirections(int);

private slots:
    void toolBtnClicked(bool);
    void lblClicked(int);
    void restartGame();
};

#endif // WIDGET_H
