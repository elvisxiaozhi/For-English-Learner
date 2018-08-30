#include "chesslbl.h"
#include <QMouseEvent>

ChessLbl::ChessLbl(QWidget *parent, int row, int col) : QLabel(parent), isCross(2)
{
    this->row = row;
    this->col = col;

    setMinimumSize(87, 87);
    setPixmap(QPixmap());
    setAlignment(Qt::AlignCenter);
    setBorder(row * 3 + col);
}

void ChessLbl::setBorder(int index)
{
    switch(index) {
    case 1:
        setStyleSheet("QLabel { border-style: solid; border-color: #29a3a3; border-width: 0px 7px; }");
        break;
    case 3:
        setStyleSheet("QLabel { border-style: solid; border-color: #29a3a3; border-width: 7px 0px; }");
        break;
    case 4:
        setStyleSheet("QLabel { border-style: solid; border-color: #29a3a3; border-width: 7px 7px; }");
        break;
    case 5:
        setStyleSheet("QLabel { border-style: solid; border-color: #29a3a3; border-width: 7px 0px; }");
        break;
    case 7:
        setStyleSheet("QLabel { border-style: solid; border-color: #29a3a3; border-width: 0px 7px; }");
        break;
    default:
        break;
    }
}

void ChessLbl::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) {
        emit clicked(row, col);
    }
}
