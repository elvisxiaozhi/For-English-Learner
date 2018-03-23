#include "bottomlabels.h"
#include <QDebug>

BottomLabels::BottomLabels()
{

}

void BottomLabels::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "Done";
    QLabel::mousePressEvent(event);
    emit labelClicked();
}
