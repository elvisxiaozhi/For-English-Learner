#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QThread>
#include "connection.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QWidget *mainWindowWidget;
    QTextEdit *contentPlace;
    QPushButton *sendButton;
    QVBoxLayout *mainWindowLayout;
    QHBoxLayout *bottomBarLayout;
    QThread *connectionThread;
    Connection *setConnection;
    QVector<QTcpSocket*> connectedClients;
    void setLayout();
    void setConnectionThread();

private slots:
    void newClientConnected();
    void sendMessages();
};

#endif // MAINWINDOW_H
