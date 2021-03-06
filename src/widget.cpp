#include "widget.h"
#include "ui_widget.h"
#include <QKeyEvent>

int Widget::crossWinningTimes = 0;
int Widget::circleWinningTimes = 0;
const int Widget::easy = 0;
const int Widget::medium = 1;
const int Widget::impossible = 2;
const int Widget::playWithAFriend = 3;
const int Widget::draw = 2;
const int Widget::notWin = 3;
const int Widget::infinityScore = 100;
const int Widget::minusInfinityScore = -100;
const int Widget::xWonScore = 10;
const int Widget::oWonScore = -10;
const int Widget::drawScore = 0;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    isXTurn = true;
    isComputerTurn = false;

    loop(3, 3, [this](int i, int){ lblArr[i] = new ChessLbl *[3]; });

    //install event filter to block signals from QComobox, so when the key is pressed, the index of QComobox will not change
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

    //set the default game mode to Medium;
    ui->difficultyMode->setCurrentIndex(impossible);

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
    loop(3, 3, [this](int i, int j){
        ChessLbl *lbl = new ChessLbl(ui->chessboard, i, j);

        lblArr[i][j] = lbl;

        ui->gridLayout->addWidget(lbl, i, j);

        connect(lbl, &ChessLbl::clicked, this, &Widget::lblClicked);
    });
}

int Widget::returnUnfilledPieces()
{
    int unfilledPieces = 0;
    loop(3, 3, [this, &unfilledPieces](int i, int j){
        if(lblArr[i][j]->isCross == ChessLbl::unfilled) {
            ++unfilledPieces;
        }
    });

    return unfilledPieces;
}

int Widget::checkWin()
{
    if(returnUnfilledPieces() == 0) {
        return draw;
    }

    //I tried to refactor this by using the loop template, but it seemed like to me below is the best way.
    for(int i = 0; i < 3; ++i) {
        for(int j = 0; j < 3; ++j) {
            if(lblArr[i][j]->isCross == ChessLbl::circle || lblArr[i][j]->isCross == ChessLbl::cross) {
                //if isWinning(int) return true, which means circle or cross has won the game
                if(isWinning(lblArr[i][j]->isCross)) {
                    return lblArr[i][j]->isCross;
                }
            }
        }
    }

    return notWin;
}

//generic function to check who is winning
bool Widget::isWinning(int isCross)
{
    QVector<std::pair<int, int> > posVec;

    //search the entire board and push back cross's or circle's coordinates(row and col) to posVec
    loop(3, 3, [this, &posVec, isCross](int i, int j){
        if(lblArr[i][j]->isCross == isCross) {
            posVec.push_back(std::make_pair(i, j));
        }
    });

    //if posVec has more than three elements, which means those three elements can make cross or circle win the game
    if(posVec.size() >= 3) {
        //search the posVec from the beginning to the last but three
        for(int i = 0; i < posVec.size() - 2; ++i) {
            //search the posVec from the second one to the last but two
            for(int j = i + 1; j < posVec.size() - 1; ++j) {
                //remember the first element and second element's coordinates difference
                int rowDiff1 = posVec[i].first - posVec[j].first;
                int colDiff1 = posVec[i].second - posVec[j].second;
                //search the posVec from the third one to the last one
                for(int k = j + 1; k < posVec.size(); ++k) {
                    //remember the second element and third element's coordinates difference
                    int rowDiff2 = posVec[j].first - posVec[k].first;
                    int colDiff2 = posVec[j].second - posVec[k].second;
                    //if the first element, second element and the third element's coordinates differences are the same,
                    //which mean cross or circle has won the game, then return true
                    if(rowDiff1 == rowDiff2 && colDiff1 == colDiff2) {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

//block tool btns signals and make it only "clickable" in some specific situations,
//like in the beginning of the game, user would like to choose circle to play with computer
void Widget::blockToolBtnSignals()
{
    //if the game mode if play with a friend, always block tool btns signals
    if(ui->difficultyMode->currentIndex() == playWithAFriend) {
        ui->cross->blockSignals(true);
        ui->circle->blockSignals(true);
    }
    //because the underline of tool btn needs to switch after each move,
    //if it's cross turn, then cross tool btn signal is unblocked, so the underline can be drawn under cross btn signal
    else if(isXTurn) { //note it's "else if"
        ui->cross->blockSignals(true);
        ui->circle->blockSignals(false);
    }
    //vice versa
    else {
        ui->cross->blockSignals(false);
        ui->circle->blockSignals(true);
    }
}

void Widget::makeComputerMove()
{
    switch (ui->difficultyMode->currentIndex()) {
    case easy:
        easyMode();
        break;
    case medium: {
        int useMinimax = rand() % 3;
        if(useMinimax > 0) {
            makeBestMove();
        }
        else {
            easyMode();
        }
        break;
    }
    case impossible: {
        makeBestMove();
        break;
    }
    default:
        break;
    }
}

void Widget::easyMode()
{
    while(true) {
        int row = rand() % 3;
        int col = rand() % 3;
        if(lblArr[row][col]->isCross == ChessLbl::unfilled) {
            lblClicked(row, col);
            break;
        }
    }
}

void Widget::makeBestMove()
{
    int score, alpha, beta;
    if(isXTurn) {
        score = minusInfinityScore;
    }
    else {
        score = infinityScore;
    }

    alpha = minusInfinityScore;
    beta = infinityScore;

    int row = 0;
    int col = 0;

    loop(3, 3, [&](int i, int j) {
        if(lblArr[i][j]->isCross == ChessLbl::unfilled) {
            lblArr[i][j]->isCross = isXTurn;

            if(isXTurn) {
                int tempScore = minSearch(0, alpha, beta);

                if(tempScore > score) {
                    score = tempScore;
                    row = i;
                    col = j;
                }
            }
            else {
                int tempScore = maxSearch(0, alpha, beta);

                if(tempScore < score) {
                    score = tempScore;
                    row = i;
                    col = j;
                }
            }

            lblArr[i][j]->isCross = ChessLbl::unfilled;
        }
    });

    lblClicked(row, col);
}

int Widget::maxSearch(int searchDepth, int alpha, int beta)
{
    if(checkWin() == xWon) {
        return xWonScore + searchDepth;
    }
    else if(checkWin() == oWon) {
        return oWonScore + searchDepth;
    }
    else if(checkWin() == draw) {
        return drawScore + searchDepth;
    }

    int score = minusInfinityScore;

    for(int i = 0; i < 3; ++i) {
        for(int j = 0; j < 3; ++j) {
            if(lblArr[i][j]->isCross == ChessLbl::unfilled) {
                lblArr[i][j]->isCross = ChessLbl::cross;

                int tempScore = minSearch(searchDepth + 1, alpha, beta);
                if(tempScore > score) {
                    score = tempScore;
                }
                if(tempScore >= beta) {
                    lblArr[i][j]->isCross = ChessLbl::unfilled;
                    return score;
                }
                if(tempScore > alpha) {
                    alpha = tempScore;
                }

                lblArr[i][j]->isCross = ChessLbl::unfilled;
            }
        }
    }

    return score + searchDepth;
}

int Widget::minSearch(int searchDepth, int alpha, int beta)
{
    if(checkWin() == xWon) {
        return xWonScore - searchDepth;
    }
    else if(checkWin() == oWon) {
        return oWonScore - searchDepth;
    }
    else if(checkWin() == draw) {
        return drawScore - searchDepth;
    }

    int score = infinityScore;

    for(int i = 0; i < 3; ++i) {
        for(int j = 0; j < 3; ++j) {
            if(lblArr[i][j]->isCross == ChessLbl::unfilled) {
                lblArr[i][j]->isCross = ChessLbl::circle;

                int tempScore = maxSearch(searchDepth + 1, alpha, beta);
                if(tempScore < score) {
                    score = tempScore;
                }
                if(tempScore <= alpha) {
                    lblArr[i][j]->isCross = ChessLbl::unfilled;
                    return score;
                }
                if(tempScore < beta) {
                    beta = tempScore;
                }

                lblArr[i][j]->isCross = ChessLbl::unfilled;
            }
        }
    }

    return score - searchDepth;
}

//block QComobox signals
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

    if(isComputerTurn) {
        isComputerTurn = false;
    }
    else {
        isComputerTurn = true;
    }

    if(returnUnfilledPieces() == 9) {
        isXTurn = true;
        makeComputerMove();
    }
}

void Widget::lblClicked(int row, int col)
{
    lblArr[row][col]->isCross = isXTurn;
    //block clciked piece(label) signal to prevent triggering lblClicked(int, int) function if users accidentally clicked filled piece(label)
    lblArr[row][col]->blockSignals(true);
    blockToolBtnSignals();

    if(isXTurn) {
        lblArr[row][col]->setPixmap(QPixmap(":/icons/cross.png"));

        ui->msLbl->setText("O Turn");
    }
    else {
        lblArr[row][col]->setPixmap(QPixmap(":/icons/circle.png"));

        ui->msLbl->setText("X Turn");
    }

    if(checkWin() == notWin) {
        toolBtnClicked(true); //this line here is very important
        if(isComputerTurn) {
            makeComputerMove();
        }
    }
    else {
        showGameOverResult();
    }
}

void Widget::showGameOverResult()
{
    ui->msLbl->setText("Game Over");

    result->show();
    result->showResult(checkWin());

    //put this after showResult(int);
    ui->cross->setText(QString::number(crossWinningTimes));
    ui->circle->setText(QString::number(circleWinningTimes));

    loop(3, 3, [this](int i, int j) { lblArr[i][j]->blockSignals(true); });
}

void Widget::restartGame()
{
    loop(3, 3, [this](int i, int j){ delete lblArr[i][j]; });

    result->hide();

    ui->msLbl->setText("Start game or select player");

    ui->cross->setStyleSheet("QToolButton#cross { border-bottom: 3px solid #00cccc; }");
    ui->circle->setStyleSheet("QToolButton#circle { border-bottom: 3px solid white; }");

    isComputerTurn = false;
    isXTurn = true;

    setLbl();

    blockToolBtnSignals();
}
