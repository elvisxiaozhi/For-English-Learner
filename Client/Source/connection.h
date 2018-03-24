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

public slots:
    void connectToServer();
};

#endif // CONNECTION_H
