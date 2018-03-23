#ifndef BOTTOMLABELS_H
#define BOTTOMLABELS_H

#include <QLabel>
#include <QMouseEvent>

class BottomLabels : public QLabel
{
    Q_OBJECT
public:
    BottomLabels();

private:
    void mousePressEvent(QMouseEvent *);

signals:
    void labelClicked();
};

#endif // BOTTOMLABELS_H
