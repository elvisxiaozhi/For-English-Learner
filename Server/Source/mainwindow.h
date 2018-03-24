#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QVBoxLayout>
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
    QVBoxLayout *mainWindowLayout;
    QThread *connectionThread;
    Connection *setConnection;
    void setLayout();
    void setConnectionThread();
};

#endif // MAINWINDOW_H
