#ifndef CHESSLBL_H
#define CHESSLBL_H

#include <QObject>
#include <QLabel>
#include <QDebug>

class ChessLbl : public QLabel
{
    Q_OBJECT
public:
    ChessLbl(QWidget *parent = nullptr, int row = 0, int col = 0);
    int isCross;

private:
    int row, col;

    void setBorder(int);

protected:
    void mousePressEvent(QMouseEvent *);

signals:
    void clicked(int, int);
};

#endif // CHESSLBL_H
