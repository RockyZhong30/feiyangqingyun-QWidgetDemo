﻿#ifndef FRMSAVERUNTIME_H
#define FRMSAVERUNTIME_H

#include <QWidget>

namespace Ui {
class frmSaveRunTime;
}

class frmSaveRunTime : public QWidget
{
    Q_OBJECT

public:
    explicit frmSaveRunTime(QWidget *parent = 0);
    ~frmSaveRunTime();

private:
    Ui::frmSaveRunTime *ui;

private slots:
    void on_checkBox_stateChanged(int arg1);
};

#endif // FRMSAVERUNTIME_H
