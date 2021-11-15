#include "calibration.h"
#include "ui_calibration.h"
#include "QDebug"

Calibration::Calibration(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Calibration)
{
    ui->setupUi(this);
}

Calibration::~Calibration()
{
    delete ui;
}

void Calibration::on_lineEdit_editingFinished()
{
    qDebug()<<"速度控制Kp已修改！";
}
