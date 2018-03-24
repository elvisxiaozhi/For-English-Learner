#include "messageboxes.h"

MessageBoxes::MessageBoxes()
{

}

void MessageBoxes::showReconnectMsBox()
{
    msBox.setText("Do you want to try reconnecting to server?");
    msBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msBox.setDefaultButton(QMessageBox::No);
    msBox.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    msBox.setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    msBox.setMinimumSize(300, 200);
    msBox.setStyleSheet("QLabel{min-width: 300px;}");
    int ret = msBox.exec();

    switch (ret) {
    case QMessageBox::Yes:
        emit reconnect();
        break;
    case QMessageBox::No:
        msBox.close();
        break;
    default:
        break;
    }
}
