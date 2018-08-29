#ifndef CHESSLBL_H
#define CHESSLBL_H

#include <QObject>
#include <QLabel>

class ChessLbl : public QLabel
{
    Q_OBJECT
public:
    ChessLbl(QWidget *parent = nullptr, int index = 1);

private:
    void setBorder(int);
};

#endif // CHESSLBL_H
