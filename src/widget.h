#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "chesslbl.h"
#include "resultwidget.h"

template<typename FUNCTION>
inline void loop(int row, int col, FUNCTION f) {
    for(int i = 0; i < row; ++i) {
        for(int j = 0; j < col; ++j) {
            f(i, j);
        }
    }
}

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
    static constexpr int xWon = 1;
    static constexpr int oWon = 0;

private:
    Ui::Widget *ui;
    ChessLbl **lblArr[3];
    bool isXTurn, isXTurnTemp;
    static const int easy;
    static const int medium;
    static const int impossible;
    static const int playWithAFriend;
    static const int draw;
    static const int notWin;
    ResultWidget *result;

    void setWidgetLayout();
    void setLbl();
    int checkWin();
    bool isWinning(int);
    void blockToolBtnSignals();
    void computerTurn();
    void easyMode();
    QVector<std::pair<std::pair<int, int>, int > > getAvaiablePlaces();
    void miniMax();
    int search();
    void findBestMove(QVector<std::pair<std::pair<int, int>, int > > &);

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    void toolBtnClicked(bool);
    void lblClicked(int, int);
    void putPiece(int, int);
    void putTestingPiece(int, int);
    void showGameOverResult();
    void restartGame();
};

#endif // WIDGET_H
