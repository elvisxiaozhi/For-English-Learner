#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setLayout();
    setConnectionThread();
    connect(setConnection->tcpServer, &QTcpServer::newConnection, this, &MainWindow::newClientConnected);
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

    bottomBarLayout = new QHBoxLayout;
    mainWindowLayout->addLayout(bottomBarLayout);

    QSpacerItem *btnSpacer = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
    bottomBarLayout->addSpacerItem(btnSpacer);
    sendButton = new QPushButton(mainWindowWidget);
    sendButton->setText("Send");
    connect(sendButton, &QPushButton::clicked, this, &MainWindow::sendMessages);
    bottomBarLayout->addWidget(sendButton);
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

void MainWindow::newClientConnected()
{
    QTcpSocket *newClientSocket = setConnection->tcpServer->nextPendingConnection();
    connectedClients.push_back(newClientSocket);
    qDebug() << "Online clients number: " << connectedClients.size();
}

void MainWindow::sendMessages()
{
    QByteArray messagesToClients;
    QDataStream out(&messagesToClients, QIODevice::WriteOnly);
    out << contentPlace->toPlainText();
    foreach (QTcpSocket *clientSocket, connectedClients) {
        clientSocket->write(messagesToClients);
    }
}
