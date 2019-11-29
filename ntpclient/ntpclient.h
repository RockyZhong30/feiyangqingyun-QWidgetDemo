﻿#ifndef NTPCLIENT_H
#define NTPCLIENT_H

/**
 * Ntp校时类 作者:feiyangqingyun(QQ:517216493) 2017-2-16
 * 1:可设置Ntp服务器IP地址
 * 2:收到时间信号发出
 */

#include <QObject>
#include <QDateTime>
class QUdpSocket;

#ifdef quc
#if (QT_VERSION < QT_VERSION_CHECK(5,7,0))
#include <QtDesigner/QDesignerExportWidget>
#else
#include <QtUiPlugin/QDesignerExportWidget>
#endif

class QDESIGNER_WIDGET_EXPORT NtpClient : public QObject
#else
class NtpClient : public QObject
#endif

{
    Q_OBJECT
public:
    static NtpClient *Instance();
    explicit NtpClient(QObject *parent = 0);

private:
    static QScopedPointer<NtpClient> self;
    QString ntpIP;
    QUdpSocket *udpSocket;

private slots:
    void readData();
    void sendData();

signals:
    //收到时间返回
    void receiveTime(const QDateTime &dateTime);

public slots:
    //设置NTP服务器IP
    void setNtpIP(const QString &ntpIP);

    //获取日期时间
    void getDateTime();

};

#endif // NTPCLIENT_H
