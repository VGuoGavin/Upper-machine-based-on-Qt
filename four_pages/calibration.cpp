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

void Calibration::sendEdit(int number,QString text){
    bool ok;
    QString base_addr= "5A00";
    QString pre_str="55aa04611104";
    int addr =base_addr.toInt(&ok,16)+number;
    QString frame=pre_str+QString("%1").arg(addr,4,16,QLatin1Char('0'))+QString("%1").arg(text.toInt(),2,16,QLatin1Char('0'))+"XXXX";
//    qDebug()<<"frame"<<frame;
    emit send_frame(frame);
}

void Calibration::on_cal_0x00_lineEdit_editingFinished()
{
    this->sendEdit(0,ui->cal_0x00_lineEdit->text());
}

void Calibration::on_cal_0x01_lineEdit_editingFinished()
{
    this->sendEdit(1,ui->cal_0x01_lineEdit->text());
}

void Calibration::on_cal_0x02_lineEdit_editingFinished()
{
    this->sendEdit(2,ui->cal_0x02_lineEdit->text());
}

void Calibration::on_cal_0x03_lineEdit_editingFinished()
{
    this->sendEdit(3,ui->cal_0x03_lineEdit->text());
}

void Calibration::on_cal_0x04_lineEdit_editingFinished()
{
    this->sendEdit(4,ui->cal_0x04_lineEdit->text());
}

void Calibration::on_cal_0x05_lineEdit_editingFinished()
{
    this->sendEdit(5,ui->cal_0x05_lineEdit->text());
}

void Calibration::on_cal_0x06_lineEdit_editingFinished()
{
    this->sendEdit(6,ui->cal_0x06_lineEdit->text());
}

void Calibration::on_cal_0x07_lineEdit_editingFinished()
{
    this->sendEdit(7,ui->cal_0x07_lineEdit->text());
}

void Calibration::on_cal_0x08_lineEdit_editingFinished()
{
    this->sendEdit(8,ui->cal_0x08_lineEdit->text());
}

void Calibration::on_cal_0x09_lineEdit_editingFinished()
{
    this->sendEdit(9,ui->cal_0x09_lineEdit->text());
}

void Calibration::on_cal_0x0A_lineEdit_editingFinished()
{
    this->sendEdit(10,ui->cal_0x0A_lineEdit->text());
}

void Calibration::on_cal_0x0B_lineEdit_editingFinished()
{
    this->sendEdit(11,ui->cal_0x0B_lineEdit->text());
}

void Calibration::on_cal_0x0C_lineEdit_editingFinished()
{
    this->sendEdit(12,ui->cal_0x0C_lineEdit->text());
}

void Calibration::on_cal_0x0D_lineEdit_editingFinished()
{
    this->sendEdit(13,ui->cal_0x0D_lineEdit->text());
}

void Calibration::on_cal_0x0E_lineEdit_editingFinished()
{
    this->sendEdit(14,ui->cal_0x0E_lineEdit->text());
}

void Calibration::on_cal_0x0F_lineEdit_editingFinished()
{
    this->sendEdit(15,ui->cal_0x0F_lineEdit->text());
}

void Calibration::on_cal_0x10_lineEdit_editingFinished()
{
    this->sendEdit(16,ui->cal_0x10_lineEdit->text());
}

void Calibration::on_cal_0x11_lineEdit_editingFinished()
{
    this->sendEdit(17,ui->cal_0x11_lineEdit->text());
}

void Calibration::on_cal_0x12_lineEdit_editingFinished()
{
    this->sendEdit(18,ui->cal_0x12_lineEdit->text());
}

void Calibration::on_cal_0x13_lineEdit_editingFinished()
{
    this->sendEdit(19,ui->cal_0x13_lineEdit->text());
}

void Calibration::on_cal_0x14_lineEdit_editingFinished()
{
    this->sendEdit(20,ui->cal_0x14_lineEdit->text());
}

void Calibration::on_cal_0x15_lineEdit_editingFinished()
{
    this->sendEdit(21,ui->cal_0x15_lineEdit->text());
}
