#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QThread>
#include "connection.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    int disabledButtonNumber;
    QWidget *mainWindowWidget;
    QTextEdit *contentPlace;
    QVBoxLayout *mainWindowLayout;
    QHBoxLayout *bottomBarLayout;
    QPushButton *bottomButtons[3];
    QThread *connectionThread;
    Connection *setConnection; //it has to be a pointer, or will show "QSocketNotifier: Socket notifiers cannot be enabled or disabled from another thread" error
    void setLayout();
    void setConnectionThread();

private slots:
    void disableBottomButtons();
};

#endif // MAINWINDOW_H
