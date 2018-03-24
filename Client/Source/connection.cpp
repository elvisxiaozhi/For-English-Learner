#include "connection.h"
#include <QDebug>

Connection::Connection(QObject *parent) : QObject(parent)
{
    tcpSocket = new QTcpSocket(this);
}

void Connection::connectToServer()
{
    tcpSocket->abort();
    tcpSocket->connectToHost(QHostAddress("127.0.0.1"), 6666);
    qDebug() << "Connecting to server";
    if(tcpSocket->waitForConnected()) {
        qDebug() << "Connected to server";
    }
    else {
        qDebug() << "Failed to connect to server.";
    }
}
