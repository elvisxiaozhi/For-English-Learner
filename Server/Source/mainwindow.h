#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QVBoxLayout>
#include "connection.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QWidget *mainWindowWidget;
    QTextEdit *contentPlace;
    QVBoxLayout *mainWindowLayout;
    void setLayout();
    Connection setConnection;
};

#endif // MAINWINDOW_H
