#include "resultwidget.h"
#include "ui_resultwidget.h"
#include "widget.h"

ResultWidget::ResultWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResultWidget)
{
    ui->setupUi(this);

    ui->cross->setPixmap(QPixmap(":/icons/cross.png"));
    ui->circle->setPixmap(QPixmap(":/icons/circle.png"));

    setStyleSheet("QLabel#result { font: 35px; color: #545454; font-weight: 900; }");
}

ResultWidget::~ResultWidget()
{
    delete ui;
}

void ResultWidget::showResult(int result)
{
    switch (result) {
    case 0:
        ui->cross->hide();
        ui->circle->show();
        ui->result->setText("WINNER!");

        ++Widget::circleWinningTimes;
        break;
    case 1:
        ui->cross->show();
        ui->circle->hide();
        ui->result->setText("WINNER!");

        ++Widget::crossWinningTimes;
        break;
    default:
        ui->cross->show();
        ui->circle->show();
        ui->result->setText("DRAW!");
        break;
    }
}
