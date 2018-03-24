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
    setWindowTitle("For English Learners(Server)");

    mainWindowWidget = new QWidget(this);
    setCentralWidget(mainWindowWidget);

    mainWindowLayout = new QVBoxLayout;
    mainWindowWidget->setLayout(mainWindowLayout);

    contentPlace = new QTextEdit(mainWindowWidget);
    contentPlace->setMinimumSize(400, 300);
    contentPlace->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainWindowLayout->addWidget(contentPlace);
}
