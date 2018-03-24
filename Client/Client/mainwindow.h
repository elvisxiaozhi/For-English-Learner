#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

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
    void setLayout();

private slots:
    void disableBottomButtons();
};

#endif // MAINWINDOW_H
