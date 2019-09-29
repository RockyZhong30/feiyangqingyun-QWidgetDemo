﻿#ifndef FRMTCPCLIENT_H
#define FRMTCPCLIENT_H

#include <QWidget>
#include <QtNetwork>

namespace Ui {
class frmTcpClient;
}

class frmTcpClient : public QWidget
{
    Q_OBJECT

public:
    explicit frmTcpClient(QWidget *parent = 0);
    ~frmTcpClient();

private:
    Ui::frmTcpClient *ui;

    bool isOk;
    QTcpSocket *tcpSocket;
    QTimer *timer;

private slots:
    void initForm();
    void initConfig();
    void saveConfig();
    void append(int type, const QString &data, bool clear = false);

    void connected();
    void disconnected();
    void readData();
    void sendData(const QString &data);

private slots:
    void on_btnConnect_clicked();
    void on_btnSave_clicked();
    void on_btnClear_clicked();
    void on_btnSend_clicked();
};

#endif // FRMTCPCLIENT_H
