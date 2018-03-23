#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setLayout();
}

MainWindow::~MainWindow()
{

}

void MainWindow::setLayout()
{
    setWindowTitle("For English Learners");

    mainWindowWidget = new QWidget(this);
    setCentralWidget(mainWindowWidget);

    mainWindowLayout = new QVBoxLayout;
    mainWindowWidget->setLayout(mainWindowLayout);

    contentPlace = new QTextEdit(mainWindowWidget);
    contentPlace->setMinimumSize(400, 300);
    contentPlace->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    contentPlace->setReadOnly(true);
    mainWindowLayout->addWidget(contentPlace);

    bottomBarLayout = new QHBoxLayout;
    mainWindowLayout->addLayout(bottomBarLayout);

    for(int i = 0; i < 3; i++) {
        bottomButtons[i] = new QPushButton(mainWindowWidget);
        bottomBarLayout->setSpacing(0);
        bottomBarLayout->addWidget(bottomButtons[i]);
    }
    bottomButtons[0]->setText("Contents");
    bottomButtons[1]->setText("Favorites");
    bottomButtons[2]->setText("About");
}
