﻿#pragma execution_character_set("utf-8")

#include "frmmaskwidget.h"
#include "ui_frmmaskwidget.h"
#include "maskwidget.h"
#include "qdialog.h"

frmMaskWidget::frmMaskWidget(QWidget *parent) : QWidget(parent), ui(new Ui::frmMaskWidget)
{
    ui->setupUi(this);
    MaskWidget::Instance()->setMainWidget(this);
    MaskWidget::Instance()->setDialogNames(QStringList() << "frmTest");
}

frmMaskWidget::~frmMaskWidget()
{
    delete ui;
    exit(0);
}

void frmMaskWidget::on_pushButton_clicked()
{
    QDialog d;
    d.setObjectName("frmTest");
    d.setWindowTitle("遮罩层弹出窗体");
    d.resize(400, 300);
    d.exec();
}
