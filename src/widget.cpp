#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    setWidgetLayout();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::setWidgetLayout()
{
    setWindowTitle("Tic Tac Toe");

    ui->cross->setIcon(QIcon(":/icons/cross.png"));
    ui->circle->setIcon(QIcon(":/icons/circle.png"));

    setStyleSheet(
                "QToolButton { background: white; border: 0px; padding: 5px 20px 2px 30px; }"
                ".QToolButton#cross { border-bottom: 3px solid #00cccc; }"
                ".QToolButton#circle { border-bottom: 3px solid white; }"
                "QLabel#msLbl { color: #666666; font: 13px; }"
                "QWidget#chessboard { background: #00cccc; }"
                "QPushButton#restartBtn { background-color: #F7F9F9; border: 0px; border-bottom: 3px solid #F7F9F9; padding: 23px 0px 17px 0px; "
                "font: 17px; color: #00b386; font-weight: 900; }"
                "QPushButton#restartBtn:pressed { border-bottom: 3px solid #A9A9A9; }"
                );
}
