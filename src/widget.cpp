#include "widget.h"
#include "ui_widget.h"
#include <QEvent>
#include <QKeyEvent>

int Widget::crossWinningTimes = 0;
int Widget::circleWinningTimes = 0;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    isXTurn = true;

    for(int i = 0; i < 3; ++i) {
        lblArr[i] = new ChessLbl *[3];
    }

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

    result = new ResultWidget(this);
    ui->gridLayout->addWidget(result);
    result->hide();

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
            ChessLbl *lbl = new ChessLbl(ui->chessboard, i, j);
            lbl->show();

            lblArr[i][j] = lbl;

            ui->gridLayout->addWidget(lbl, i, j);

            connect(lbl, &ChessLbl::clicked, this, &Widget::lblClicked);
        }
    }
}

int Widget::checkWin()
{
    int filledSpace = 0;

    for(int i = 0; i < 3; ++i) {
        for(int j = 0; j < 3; ++j) {
            if(lblArr[i][j]->isCross == 0 || lblArr[i][j]->isCross == 1) {
                if(isWinning(lblArr[i][j]->isCross)) {
                    return lblArr[i][j]->isCross;
                }

                ++filledSpace;
            }
        }
    }

    if(filledSpace == 9) {
        return 2;
    }

    return 3;
}

bool Widget::isWinning(int isCross)
{
    QVector<std::pair<int, int> > posVec;
    for(int i = 0; i < 3; ++i) {
        for(int j = 0; j < 3; ++j) {
            if(lblArr[i][j]->isCross == isCross) {
                posVec.push_back(std::make_pair(i, j));
            }
        }
    }

    if(posVec.size() >= 3) {
        for(int i = 0; i < posVec.size() - 2; ++i) {
            for(int j = i + 1; j < posVec.size() - 1; ++j) {
                int rowDiff1 = posVec[i].first - posVec[j].first;
                int colDiff1 = posVec[i].second - posVec[j].second;
                for(int k = j + 1; k < posVec.size(); ++k) {
                    int rowDiff2 = posVec[j].first - posVec[k].first;
                    int colDiff2 = posVec[j].second - posVec[k].second;
                    if(rowDiff1 == rowDiff2 && colDiff1 == colDiff2) {
                        return true;
                    }
                }
            }
        }
    }

    return false;
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

void Widget::lblClicked(int row, int col)
{
    if(lblArr[row][col]->pixmap()->isNull() && checkWin() == 3) {
        if(isXTurn) {
            lblArr[row][col]->setPixmap(QPixmap(":/icons/cross.png"));
            lblArr[row][col]->isCross = 1;

            emit ui->circle->click();

            ui->msLbl->setText("O Turn");

        }
        else {
            lblArr[row][col]->setPixmap(QPixmap(":/icons/circle.png"));
            lblArr[row][col]->isCross = 0;

            emit ui->cross->click();

            ui->msLbl->setText("X Turn");
        }

        if(checkWin() != 3) {
            ui->msLbl->setText("Game Over");

            for(int i = 0; i < 3; ++i) {
                for(int j = 0; j < 3; ++j) {
                    lblArr[i][j]->hide();
                }
            }

            result->show();
            result->showResult(checkWin());

            //put this after showResult(int);
            ui->cross->setText(QString::number(crossWinningTimes));
            ui->circle->setText(QString::number(circleWinningTimes));
        }
    }
}

void Widget::restartGame()
{
    for(int i = 0; i < 3; ++i) {
        for(int j = 0; j < 3; ++j) {
            delete lblArr[i][j];
        }
    }

    result->hide();

    ui->msLbl->setText("Start game or select player");

    ui->cross->setStyleSheet("QToolButton#cross { border-bottom: 3px solid #00cccc; }");
    ui->circle->setStyleSheet("QToolButton#circle { border-bottom: 3px solid white; }");

    isXTurn = true;

    setLbl();
}
