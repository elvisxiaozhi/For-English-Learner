#include "connection.h"
#include <QDebug>

Connection::Connection(QObject *parent) : QObject(parent)
{
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &Connection::getError);

}

void Connection::connectToServer()
{
    tcpSocket->abort();
    tcpSocket->connectToHost(QHostAddress("127.0.0.1"), 6666);
    qDebug() << "Connecting to server";
    if(tcpSocket->waitForConnected()) {
        qDebug() << "Connected to server";
    }
}

void Connection::getError(QAbstractSocket::SocketError error)
{
    qDebug() << "Error: " << error;
    if(error == 0) {
        emit connectionRefusedError();
    }
    if(error == 1) {
        emit remoteHostClosedError();
    }
}
