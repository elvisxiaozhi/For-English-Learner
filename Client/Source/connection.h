#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QtNetwork>

class Connection : public QObject
{
    Q_OBJECT
public:
    explicit Connection(QObject *parent = nullptr);

    QTcpSocket *tcpSocket;

signals:
    void failedToConnect();
    void connectionRefusedError();
    void remoteHostClosedError();

public slots:
    void connectToServer();
    void getError(QAbstractSocket::SocketError);
};

#endif // CONNECTION_H
