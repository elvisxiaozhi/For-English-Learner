#ifndef MESSAGEBOXES_H
#define MESSAGEBOXES_H

#include <QMessageBox>

class MessageBoxes : public QObject
{
    Q_OBJECT
public:
    MessageBoxes();
    QMessageBox msBox;
    void showReconnectMsBox();

signals:
    void reconnect();
};

#endif // MESSAGEBOXES_H
