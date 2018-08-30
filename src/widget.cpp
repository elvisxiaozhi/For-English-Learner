#include "widget.h"
#include "ui_widget.h"
#include <QEvent>
#include <QKeyEvent>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    isXTurn = true;

    setWidgetLayout();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::setWidgetLayout()
{
    setWindowTitle("Tic Tac Toe");

    ui->difficultyMode->addItem("Easy");
    ui->difficultyMode->addItem("Medium");
    ui->difficultyMode->addItem("Impossible");
    ui->difficultyMode->addItem("Play against a friend");

    ui->difficultyMode->setCurrentIndex(1);

    ui->cross->setIcon(QIcon(":/icons/cross.png"));
    ui->circle->setIcon(QIcon(":/icons/circle.png"));

    setLbl();

    setStyleSheet(
                "QComboBox { border: 0px; padding: 8px 8px 8px 8px; color: #696969; font: 15px; font-weight: 700; }"
                "QComboBox::drop-down { border: 0px; }"
                "QComboBox::down-arrow { image: url(:/icons/down-arrow.png); }"
                "QComboBox::down-arrow:on { image: url(:/icons/up-arrow.png); }"
                "QComboBox QAbstractItemView { border: 0px; selection-background-color: #D3D3D3; color: #A9A9A9; }"
                "QToolButton { background: white; border: 0px; padding: 5px 20px 2px 30px; font: 25px; color: #696969; }"
                "QToolButton#cross { border-bottom: 3px solid #00cccc; }"
                "QToolButton#circle { border-bottom: 3px solid white; }"
                "QLabel#msLbl { color: #666666; font: 13px; }"
                "QWidget#chessboard { background: #00cccc; }"
                "QPushButton#restartBtn { background-color: #F7F9F9; border: 0px; border-bottom: 3px solid #F7F9F9; padding: 23px 0px 17px 0px; "
                "font: 17px; color: #00b386; font-weight: 900; }"
                "QPushButton#restartBtn:pressed { border-bottom: 3px solid #00cccc; }" //#A9A9A9
                );

    connect(ui->cross, &QToolButton::clicked, this, &Widget::toolBtnClicked);
    connect(ui->circle, &QToolButton::clicked, this, &Widget::toolBtnClicked);
    connect(ui->restartBtn, &QPushButton::clicked, this, &Widget::restartGame);
}

void Widget::setLbl()
{
    for(int i = 0; i < 3; ++i) {
        for(int j = 0; j < 3; ++j) {
            ChessLbl *lbl = new ChessLbl(ui->chessboard, 3 * i + j);
            lblVec.push_back(lbl);
            ui->gridLayout->addWidget(lbl, i, j);

            connect(lbl, &ChessLbl::clicked, this, &Widget::lblClicked);
        }
    }
}

void Widget::checkWin()
{
    int filledSpace = 0;
    for(int i = 0; i < lblVec.size(); ++i) {
        if(lblVec[i]->isCross == 0 || lblVec[i]->isCross == 1) {
            checkEightDirections(i);
        }
        else {
            ++filledSpace;
            if(filledSpace == 9) {
                qDebug() << "Draw";
                break;
            }
        }
    }
}

int Widget::checkEightDirections(int index)
{
    for(int i = -4; i < 5; ++i) {
        if(i == 0) {
            continue;
        }
        else {
            if(index + i >= 0 && index + i <= 8) {
                if(lblVec[index + i]->isCross == lblVec[index]->isCross) {
                    if(index + i + i >= 0 && index + i + i <= 8) {
                        if(lblVec[index + i + i]->isCross == lblVec[index]->isCross) {
                            return lblVec[index]->isCross;
                        }
                    }
                }
            }
        }
    }
    return 2;
}

void Widget::toolBtnClicked(bool)
{
    if(sender()->objectName() == "cross") {
        ui->cross->setStyleSheet("QToolButton#cross { border-bottom: 3px solid #00cccc; }");
        ui->circle->setStyleSheet("QToolButton#circle { border-bottom: 3px solid white; }");

        isXTurn = true;
    }
    else {
        ui->cross->setStyleSheet("QToolButton#cross { border-bottom: 3px solid white; }");
        ui->circle->setStyleSheet("QToolButton#circle { border-bottom: 3px solid #00cccc; }");

        isXTurn = false;
    }
}

void Widget::lblClicked(int index)
{
    if(lblVec[index]->pixmap()->isNull()) {
        if(isXTurn) {
            lblVec[index]->setPixmap(QPixmap(":/icons/cross.png"));
            lblVec[index]->isCross = 1;

            emit ui->circle->click();

            ui->msLbl->setText("O Turn");

        }
        else {
            lblVec[index]->setPixmap(QPixmap(":/icons/circle.png"));
            lblVec[index]->isCross = 0;

            emit ui->cross->click();

            ui->msLbl->setText("X Turn");
        }

        checkWin();
    }
}

void Widget::restartGame()
{
    for(int i = 0; i < lblVec.size(); ++i) {
        delete lblVec[i];
    }
    lblVec.clear();

    ui->msLbl->setText("Start game or select player");

    ui->cross->setStyleSheet("QToolButton#cross { border-bottom: 3px solid #00cccc; }");
    ui->circle->setStyleSheet("QToolButton#circle { border-bottom: 3px solid white; }");

    isXTurn = true;

    setLbl();
}
