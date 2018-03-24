#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setLayout();
    setConnectionThread();
    connect(setConnection->tcpServer, &QTcpServer::newConnection, this, &MainWindow::sendMessages);
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

void MainWindow::setConnectionThread()
{
    connectionThread = new QThread;
    setConnection = new Connection();
    setConnection->moveToThread(connectionThread);
    connect(connectionThread, &QThread::finished, connectionThread, &QThread::deleteLater);
    connect(connectionThread, &QThread::finished, setConnection, &QObject::deleteLater);
    connectionThread->start();
}

void MainWindow::sendMessages()
{
    QByteArray messagesToClients;
    QDataStream out(&messagesToClients, QIODevice::WriteOnly);
    out << contentPlace->toPlainText();
    QTcpSocket *tcpSocket = setConnection->tcpServer->nextPendingConnection();
    tcpSocket->write(messagesToClients);
}
