﻿#ifndef FRMUDPCLIENT_H
#define FRMUDPCLIENT_H

#include <QWidget>
#include <QtNetwork>

namespace Ui {
class frmUdpClient;
}

class frmUdpClient : public QWidget
{
    Q_OBJECT

public:
    explicit frmUdpClient(QWidget *parent = 0);
    ~frmUdpClient();

private:
    Ui::frmUdpClient *ui;

    QUdpSocket *udpSocket;
    QTimer *timer;

private slots:
    void initForm();
    void initConfig();
    void saveConfig();
    void changeTimer();
    void append(int type, const QString &data, bool clear = false);

    void readData();
    void sendData(const QString &ip, int port, const QString &data);

private slots:
    void on_btnSave_clicked();
    void on_btnClear_clicked();
    void on_btnSend_clicked();
};

#endif // FRMUDPCLIENT_H
