#include "connection.h"

Connection::Connection(QObject *parent) : QObject(parent)
{
    tcpServer = new QTcpServer; //can not set parent there, or error will show
    tcpServer->listen(QHostAddress::Any, 6666);
}
