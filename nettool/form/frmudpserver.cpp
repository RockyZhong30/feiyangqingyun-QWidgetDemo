﻿#include "frmudpserver.h"
#include "ui_frmudpserver.h"
#include "quiwidget.h"

frmUdpServer::frmUdpServer(QWidget *parent) : QWidget(parent), ui(new Ui::frmUdpServer)
{
    ui->setupUi(this);
    this->initForm();
    this->initIP();
    this->initConfig();
}

frmUdpServer::~frmUdpServer()
{
    delete ui;
}

void frmUdpServer::initForm()
{
    udpSocket = new QUdpSocket(this);
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(readData()));

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(on_btnSend_clicked()));

    ui->cboxInterval->addItems(App::Intervals);
    ui->cboxData->addItems(App::Datas);
    QUIHelper::setLabStyle(ui->labCount, 3);
}

void frmUdpServer::initIP()
{
    //获取本机所有IP
    QStringList ips;
    QList<QNetworkInterface> netInterfaces = QNetworkInterface::allInterfaces();
    foreach(const QNetworkInterface  &netInterface, netInterfaces) {
        //移除虚拟机和抓包工具的虚拟网卡
        QString humanReadableName = netInterface.humanReadableName().toLower();
        if(humanReadableName.startsWith("vmware network adapter") || humanReadableName.startsWith("npcap loopback adapter")) {
            continue;
        }

        //过滤当前网络接口
        bool flag = (netInterface.flags() == (QNetworkInterface::IsUp | QNetworkInterface::IsRunning | QNetworkInterface::CanBroadcast | QNetworkInterface::CanMulticast));
        if(flag) {
            QList<QNetworkAddressEntry> addrs = netInterface.addressEntries();
            foreach(QNetworkAddressEntry addr, addrs) {
                //只取出IPV4的地址
                if(addr.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                    QString ip4 = addr.ip().toString();
                    if(ip4 != "127.0.0.1") {
                        ips.append(ip4);
                    }
                }
            }
        }
    }

    ui->cboxListenIP->addItems(ips);
    ui->cboxListenIP->addItem("127.0.0.1");
}

void frmUdpServer::initConfig()
{
    ui->ckHexSend->setChecked(App::HexSendUdpServer);
    connect(ui->ckHexSend, SIGNAL(stateChanged(int)), this, SLOT(saveConfig()));

    ui->ckHexReceive->setChecked(App::HexReceiveUdpServer);
    connect(ui->ckHexReceive, SIGNAL(stateChanged(int)), this, SLOT(saveConfig()));

    ui->ckAscii->setChecked(App::AsciiUdpServer);
    connect(ui->ckAscii, SIGNAL(stateChanged(int)), this, SLOT(saveConfig()));

    ui->ckDebug->setChecked(App::DebugUdpServer);
    connect(ui->ckDebug, SIGNAL(stateChanged(int)), this, SLOT(saveConfig()));

    ui->ckAutoSend->setChecked(App::AutoSendUdpServer);
    connect(ui->ckAutoSend, SIGNAL(stateChanged(int)), this, SLOT(saveConfig()));

    ui->cboxInterval->setCurrentIndex(ui->cboxInterval->findText(QString::number(App::IntervalUdpServer)));
    connect(ui->cboxInterval, SIGNAL(currentIndexChanged(int)), this, SLOT(saveConfig()));

    ui->cboxListenIP->setCurrentIndex(ui->cboxListenIP->findText(App::UdpListenIP));
    connect(ui->cboxListenIP, SIGNAL(currentIndexChanged(int)), this, SLOT(saveConfig()));

    ui->txtListenPort->setText(QString::number(App::UdpListenPort));
    connect(ui->txtListenPort, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));

    ui->ckSelectAll->setChecked(App::SelectAllUdpServer);
    connect(ui->ckSelectAll, SIGNAL(stateChanged(int)), this, SLOT(saveConfig()));

    this->changeTimer();
}

void frmUdpServer::saveConfig()
{
    App::HexSendUdpServer = ui->ckHexSend->isChecked();
    App::HexReceiveUdpServer = ui->ckHexReceive->isChecked();
    App::AsciiUdpServer = ui->ckAscii->isChecked();
    App::DebugUdpServer = ui->ckDebug->isChecked();
    App::AutoSendUdpServer = ui->ckAutoSend->isChecked();
    App::IntervalUdpServer = ui->cboxInterval->currentText().toInt();
    App::UdpListenIP = ui->cboxListenIP->currentText();
    App::UdpListenPort = ui->txtListenPort->text().trimmed().toInt();
    App::SelectAllUdpServer = ui->ckSelectAll->isChecked();
    App::writeConfig();

    this->changeTimer();
}

void frmUdpServer::changeTimer()
{
    timer->setInterval(App::IntervalUdpServer);
    if (App::AutoSendUdpServer) {
        if (!timer->isActive()) {
            timer->start();
        }
    } else {
        if (timer->isActive()) {
            timer->stop();
        }
    }
}

void frmUdpServer::append(int type, const QString &data, bool clear)
{
    static int currentCount = 0;
    static int maxCount = 100;

    if (clear) {
        ui->txtMain->clear();
        currentCount = 0;
        return;
    }

    if (currentCount >= maxCount) {
        ui->txtMain->clear();
        currentCount = 0;
    }

    if (ui->ckShow->isChecked()) {
        return;
    }

    //过滤回车换行符
    QString strData = data;
    strData = strData.replace("\r", "");
    strData = strData.replace("\n", "");

    //不同类型不同颜色显示
    QString strType;
    if (type == 0) {
        strType = "发送";
        ui->txtMain->setTextColor(QColor("darkgreen"));
    } else {
        strType = "接收";
        ui->txtMain->setTextColor(QColor("red"));
    }

    strData = QString("时间[%1] %2: %3").arg(TIMEMS).arg(strType).arg(strData);
    ui->txtMain->append(strData);
    currentCount++;
}

void frmUdpServer::readData()
{
    QHostAddress host;
    quint16 port;
    QByteArray data;
    QString buffer;

    while (udpSocket->hasPendingDatagrams()) {
        data.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(data.data(), data.size(), &host, &port);

        if (App::HexReceiveUdpServer) {
            buffer = QUIHelper::byteArrayToHexStr(data);
        } else if (App::AsciiUdpServer) {
            buffer = QUIHelper::byteArrayToAsciiStr(data);
        } else {
            buffer = QString(data);
        }

        QString ip = host.toString();
        ip = ip.replace("::ffff:", "");
        if (ip.isEmpty()) {
            continue;
        }

        QString str = QString("[%1:%2] %3").arg(ip).arg(port).arg(buffer);
        append(1, str);
        clientConnected(ip, port);

        if (App::DebugUdpServer) {
            int count = App::Keys.count();
            for (int i = 0; i < count; i++) {
                if (App::Keys.at(i) == buffer) {
                    sendData(ip, port, App::Values.at(i));
                    break;
                }
            }
        }
    }
}

void frmUdpServer::sendData(const QString &ip, int port, const QString &data)
{
    QByteArray buffer;
    if (App::HexSendUdpServer) {
        buffer = QUIHelper::hexStrToByteArray(data);
    } else if (App::AsciiUdpServer) {
        buffer = QUIHelper::asciiStrToByteArray(data);
    } else {
        buffer = data.toLatin1();
    }

    udpSocket->writeDatagram(buffer, QHostAddress(ip), port);

    QString str = QString("[%1:%2] %3").arg(ip).arg(port).arg(data);
    append(0, str);
}

void frmUdpServer::clientConnected(const QString &ip, int port)
{
    //先过滤重复的
    QString str = QString("%1:%2").arg(ip).arg(port);
    for (int i = 0; i < ui->listWidget->count(); i++) {
        QString s = ui->listWidget->item(i)->text();
        if (str == s) {
            return;
        }
    }

    ui->listWidget->addItem(str);
    ui->labCount->setText(QString("共 %1 个客户端").arg(ui->listWidget->count()));
}

void frmUdpServer::on_btnListen_clicked()
{
    if (ui->btnListen->text() == "监听") {
        bool ok = udpSocket->bind(QHostAddress(App::UdpListenIP), App::UdpListenPort);
        if (ok) {
            ui->btnListen->setText("关闭");
            append(0, "监听成功");
        }
    } else {
        udpSocket->abort();
        ui->btnListen->setText("监听");
    }
}

void frmUdpServer::on_btnSave_clicked()
{
    QString data = ui->txtMain->toPlainText();
    if (data.length() <= 0) {
        return;
    }

    QString fileName = QString("%1/%2.txt").arg(QUIHelper::appPath()).arg(STRDATETIME);
    QFile file(fileName);
    if (file.open(QFile::WriteOnly | QFile::Text)) {
        file.write(data.toUtf8());
        file.close();
    }

    on_btnClear_clicked();
}

void frmUdpServer::on_btnClear_clicked()
{
    append(0, "", true);
}

void frmUdpServer::on_btnSend_clicked()
{
    QString data = ui->cboxData->currentText();
    if (data.length() <= 0) {
        return;
    }

    if (ui->ckSelectAll->isChecked()) {
        for (int i = 0; i < ui->listWidget->count(); i++) {
            QString str = ui->listWidget->item(i)->text();
            QStringList list = str.split(":");
            sendData(list.at(0), list.at(1).toInt(), data);
        }
    } else {
        int row = ui->listWidget->currentRow();
        if (row >= 0) {
            QString str = ui->listWidget->item(row)->text();
            QStringList list = str.split(":");
            sendData(list.at(0), list.at(1).toInt(), data);
        }
    }
}
