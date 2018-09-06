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
        case 1:
            easyMode();
            break;
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

QMap<std::pair<int, int>, int> Widget::getAvaiablePlaces()
{
    QMap<std::pair<int, int>, int> scoreMap;
    for(int i = 0; i < 3; ++i) {
        for(int j = 0; j < 3; ++j) {
            if(lblArr[i][j]->isCross == 2) {
                scoreMap.insert(std::make_pair(i, j), 0);
            }
        }
    }
    return scoreMap;
}

void Widget::miniMax()
{
    int **board = getCurrentBoard();
    QMap<std::pair<int, int>, int> score = getAvaiablePlaces();

    scoreMap(score, board);

//    findBestScore(score);

    qDebug() << score;
}

int **Widget::getCurrentBoard()
{
    int **board = nullptr;
    board = new int *[3];
    for(int i = 0; i < 3; ++i) {
        board[i] = new int[3];
        for(int j = 0; j < 3; ++j) {
            board[i][j] = lblArr[i][j]->isCross;
        }
    }
    return board;
}

void Widget::restoreBoard(int **board)
{
    for(int i = 0; i < 3; ++i) {
        for(int j = 0; j < 3; ++j) {
            lblArr[i][j]->isCross = board[i][j];
        }
    }
}

void Widget::scoreMap(QMap<std::pair<int, int>, int> &score, int **board)
{
    for(auto e : score.toStdMap()) {
//        putLblOnBoard(e.first.first, e.first.second);
        if(checkWin() == 0) {
            score.insert(std::make_pair(e.first.first, e.first.second), e.second - 10);

            restoreBoard(board);
        }
        else if(checkWin() == 1) {
            score.insert(std::make_pair(e.first.first, e.first.second), e.second + 10);

            restoreBoard(board);
        }
        else if(checkWin() == 3) {
            int **board2 = getCurrentBoard();
            QMap<std::pair<int, int>, int> score2 = getAvaiablePlaces();
            scoreMap(score2, board2);
            score.insert(std::make_pair(e.first.first, e.first.second), e.second + findBestScore(score2));
            restoreBoard(board2);
            delete []board2;
        }
    }

    findBestScore(score);
}

int Widget::findBestScore(QMap<std::pair<int, int>, int> &map)
{
    auto it = std::max_element(map.begin(), map.end(),
        [](int a, int b) {
            return a < b;
        });
    return it.value();
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

    blockToolBtnSignals();
}
