#include "widget.h"
#include "ui_widget.h"
#include <QKeyEvent>

int Widget::crossWinningTimes = 0;
int Widget::circleWinningTimes = 0;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    isXTurn = true;
    isXTurnTemp = true;

    for(int i = 0; i < 3; ++i) {
        lblArr[i] = new ChessLbl *[3];
    }

    ui->difficultyMode->installEventFilter(this);

    setWidgetLayout();

    connect(ui->difficultyMode, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int) { restartGame(); });
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
    ui->gridLayout->addWidget(result, 0, 3, 3, 3);
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

void Widget::blockToolBtnSignals()
{
    if(ui->difficultyMode->currentIndex() == 3) {
        ui->cross->blockSignals(true);
        ui->circle->blockSignals(true);
    }

    for(int i = 0; i < 3; ++i) {
        for(int j = 0; j < 3; ++j) {
            if(lblArr[i][j]->isCross != 2) {
                if(isXTurn) {
                    ui->cross->blockSignals(true);
                    ui->circle->blockSignals(false);
                }
                else {
                    ui->cross->blockSignals(false);
                    ui->circle->blockSignals(true);
                }
                return;
            }
        }
    }
}

void Widget::computerTurn()
{
    if(checkWin() == 3) {
        switch (ui->difficultyMode->currentIndex()) {
        case 0:
            easyMode();
            break;
//        case 1:
//            easyMode();
//            break;
        default:
            break;
        }
    }
}

void Widget::easyMode()
{
    while(true) {
        int row = rand() % 3;
        int col = rand() % 3;
        if(lblArr[row][col]->isCross == 2) {
            if(isXTurn) {
                lblArr[row][col]->setPixmap(QPixmap(":/icons/cross.png"));
                lblArr[row][col]->isCross = 1;
            }
            else {
                lblArr[row][col]->setPixmap(QPixmap(":/icons/circle.png"));
                lblArr[row][col]->isCross = 0;
            }
            break;
        }
    }
}

QVector<std::pair<std::pair<int, int>, int > > Widget::getAvaiablePlaces()
{
    QVector<std::pair<std::pair<int, int>, int > > scoreVec;
    for(int i = 0; i < 3; ++i) {
        for(int j = 0; j < 3; ++j) {
            if(lblArr[i][j]->isCross == 2) {
                scoreVec.push_back(std::make_pair(std::make_pair(i, j), 0));
            }
        }
    }

    return scoreVec;
}

void Widget::miniMax()
{
    isXTurnTemp = isXTurn;

    QVector<std::pair<std::pair<int, int>, int > > score = getAvaiablePlaces();

    for(int i = 0; i < score.size(); ++i) {
        lblArr[score[i].first.first][score[i].first.second]->isCross = isXTurn;
        toolBtnClicked(true);

        score[i].second = search();

        lblArr[score[i].first.first][score[i].first.second]->isCross = 2;
        isXTurn = isXTurnTemp;
    }

    qDebug() << score << isXTurn;

    findBestMove(score);
}

int Widget::search()
{
    if(checkWin() == 0) {
        return -10;
    }
    if(checkWin() == 1) {
        return 10;
    }
    if(checkWin() == 2) {
        return 0;
    }

    int score = 0;
    int depth = 0;

    for(int i = 0; i < 3; ++i) {
        for(int j = 0; j < 3; ++j) {
            if(lblArr[i][j]->isCross == 2) {
                toolBtnClicked(true);
                lblArr[i][j]->isCross = isXTurn;

                if(isXTurn) {
                    score = std::max(score, search());
                }
                else {
                    score = std::min(score, search());
                }

                ++depth;

                lblArr[i][j]->isCross = 2;
            }
        }
    }

    if(isXTurn) {
        score -= depth;
    }
    else {
        score += depth;
    }

    qDebug() << score;

    return score;
}

void Widget::findBestMove(QVector<std::pair<std::pair<int, int>, int> > &vec)
{
    int row = vec[0].first.first;
    int col = vec[0].first.second;

    for(int i = 0; i < vec.size() - 1; ++i) {
        if(isXTurn) {
            if(vec[i].second < vec[i + 1].second) {
                row = vec[i + 1].first.first;
                col = vec[i + 1].first.second;
            }
        }
        else {
            if(vec[i].second > vec[i + 1].second) {
                row = vec[i + 1].first.first;
                col = vec[i + 1].first.second;
            }
        }
    }

    lblClicked(row, col);
}

bool Widget::eventFilter(QObject *watched, QEvent *event)
{
    if(ui->difficultyMode) {
        if(ui->difficultyMode == watched && event->type() == QEvent::KeyPress){
           QKeyEvent *key = static_cast<QKeyEvent *>(event);
           if(!key->text().isEmpty())
                return true;
        }
    }

    return QObject::eventFilter(watched, event);
}

void Widget::toolBtnClicked(bool)
{
    if(isXTurn) {
        ui->cross->setStyleSheet("QToolButton#cross { border-bottom: 3px solid white; }");
        ui->circle->setStyleSheet("QToolButton#circle { border-bottom: 3px solid #00cccc; }");

        isXTurn = false;
    }
    else {
        ui->cross->setStyleSheet("QToolButton#cross { border-bottom: 3px solid #00cccc; }");
        ui->circle->setStyleSheet("QToolButton#circle { border-bottom: 3px solid white; }");

        isXTurn = true;
    }

    computerTurn();
}

void Widget::lblClicked(int row, int col)
{
    if(lblArr[row][col]->pixmap()->isNull() && checkWin() == 3) {
        if(isXTurn) {
            lblArr[row][col]->setPixmap(QPixmap(":/icons/cross.png"));
            lblArr[row][col]->isCross = 1;

            blockToolBtnSignals();

            emit ui->circle->click();

            ui->msLbl->setText("O Turn");

        }
        else {
            lblArr[row][col]->setPixmap(QPixmap(":/icons/circle.png"));
            lblArr[row][col]->isCross = 0;

            blockToolBtnSignals();

            emit ui->cross->click();

            ui->msLbl->setText("X Turn");
        }

        if(checkWin() != 3) {
            ui->msLbl->setText("Game Over");

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
//    for(int i = 0; i < 3; ++i) {
//        for(int j = 0; j < 3; ++j) {
//            delete lblArr[i][j];
//        }
//    }

//    result->hide();

//    ui->msLbl->setText("Start game or select player");

//    ui->cross->setStyleSheet("QToolButton#cross { border-bottom: 3px solid #00cccc; }");
//    ui->circle->setStyleSheet("QToolButton#circle { border-bottom: 3px solid white; }");

//    isXTurn = true;

//    setLbl();

//    blockToolBtnSignals();
    miniMax();
}
