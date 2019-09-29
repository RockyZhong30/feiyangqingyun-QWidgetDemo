﻿#ifndef FRMMAIN_H
#define FRMMAIN_H

#include <QWidget>

namespace Ui {
class frmMain;
}

class frmMain : public QWidget
{
    Q_OBJECT

public:
    explicit frmMain(QWidget *parent = 0);
    ~frmMain();

private slots:
    void on_tabWidget_currentChanged(int index);

private:
    Ui::frmMain *ui;
};

#endif // FRMMAIN_H
