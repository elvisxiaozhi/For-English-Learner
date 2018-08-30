#ifndef CHESSLBL_H
#define CHESSLBL_H

#include <QObject>
#include <QLabel>
#include <QDebug>

class ChessLbl : public QLabel
{
    Q_OBJECT
public:
    ChessLbl(QWidget *parent = nullptr, int index = 1);
    int isCross;

private:
    int index;

    void setBorder(int);

protected:
    void mousePressEvent(QMouseEvent *);

signals:
    void clicked(int);
};

#endif // CHESSLBL_H
