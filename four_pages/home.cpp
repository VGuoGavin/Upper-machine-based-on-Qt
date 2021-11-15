#include "home.h"
#include "ui_home.h"
#include <iostream>
#include <QDialog>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QDateTime>
#include "dev/frame.h"
#include <QMessageBox>
#include <QList>

Frame *f=new Frame();
QString FRAME="";
Home::Home(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Home)
{
    ui->setupUi(this);
    //查找可用的串口
    this->Find_Port();
    //设置波特率下拉菜单默认显示第0项
    ui->BaudBox->setCurrentIndex(4);
    int BaudIndex=ui->BaudBox->currentIndex();
    switch (BaudIndex) {
    case 0:
        ui->BaudBox->setCurrentIndex(0);break;
    case 1:
        ui->BaudBox->setCurrentIndex(1);break;
    case 2:
        ui->BaudBox->setCurrentIndex(2);break;
    case 3:
        ui->BaudBox->setCurrentIndex(3);break;
    case 5:
        ui->BaudBox->setCurrentIndex(4);break;
    default:
        ui->BaudBox->setCurrentIndex(4);break;
    }

    QAction *pActionTest = new QAction();

    ui->OpenSerialButton->addAction(pActionTest);

    connect(pActionTest,&QAction::triggered,this,&Home::OnActionTest);

    ui->SendButton->setDisabled(true);

}

Home::~Home()
{
    delete ui;
}
void Home::on_FindPort_clicked(){
    this->Find_Port();
}

void Home::Find_Port(){
    //查找可用的串口
    int res=QSerialPortInfo::availablePorts().count();
//    qDebug()<<"串口数量"<<res;
    if(res==0){
        emit send_operation_messgae(0,"未找到可用端口",Qt::red);
        ui->OpenSerialButton->setDisabled(true);
    }else{

        foreach (const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
        {
            QSerialPort serial;
            serial.setPort(info);
            //qDebug()<<res;

            if(serial.open(QIODevice::ReadWrite))
            {
                ui->PortBox->addItem(serial.portName());
                serial.close();
            }
            emit send_operation_messgae(0,"成功找到串口:"+serial.portName(),Qt::blue);
        }
        ui->OpenSerialButton->setDisabled(false);
    }
}

//菜单响应函数
void Home::OnActionTest()
{
    QDialog dlg;
    dlg.setWindowTitle("测试菜单响应");
    dlg.exec();
}

void Home::on_OpenSerialButton_clicked()
{
    if(ui->OpenSerialButton->text() == tr("打开串口"))
    {
        ui->SendButton->setDisabled(false); //发送按钮可以使用
        serial = new QSerialPort;
        //设置串口名
        serial->setPortName(ui->PortBox->currentText());
        //打开串口
        bool res= serial->open(QIODevice::ReadWrite);
        if(res==0){
            emit send_operation_messgae(0,"连接失败",Qt::red);

        }else{
            emit send_operation_messgae(0,"连接成功",Qt::blue);
            emit send_upmachine_state(1);
        }
        //设置波特率
        int BaudIndex=ui->BaudBox->currentIndex();
        //设置波特率
        switch (BaudIndex) {
        case 0:
            serial->setBaudRate(QSerialPort::Baud9600);break;
        case 1:
            serial->setBaudRate(QSerialPort::Baud115200);break;
        case 2:
            serial->setBaudRate(QSerialPort::Baud38400);break;
        case 3:
            serial->setBaudRate(QSerialPort::Baud57600);break;
        case 5:
            serial->setBaudRate(QSerialPort::Baud115200);break;
        default:
            serial->setBaudRate(QSerialPort::Baud115200);break;
        }

        //设置数据位数
        switch (ui->BitBox->currentIndex())
        {
        case 8:
            serial->setDataBits(QSerialPort::Data8);//设置数据位8
            break;
        default:
            break;
        }
        //设置校验位
        switch (ui->ParityBox->currentIndex())
        {
        case 0:
            serial->setParity(QSerialPort::NoParity);
            break;
        default:
            break;
        }
        //设置停止位
        switch (ui->BitBox->currentIndex())
        {
        case 1:
            serial->setStopBits(QSerialPort::OneStop);//停止位设置为1
            break;
        case 2:
            serial->setStopBits(QSerialPort::TwoStop);
        default:
            break;
        }
        //设置流控制
        serial->setFlowControl(QSerialPort::NoFlowControl);//设置为无流控制

        //关闭设置菜单使能
        ui->PortBox->setEnabled(false);
        ui->BaudBox->setEnabled(false);
        ui->BitBox->setEnabled(false);
        ui->ParityBox->setEnabled(false);
        ui->StopBox->setEnabled(false);
        ui->OpenSerialButton->setText(tr("关闭串口"));

        //连接信号槽
        QObject::connect(serial,&QSerialPort::readyRead,this,&Home::ReadData);
        emit send_start_or_not(true);

    }
    else
    {
        ui->SendButton->setDisabled(true);
        //关闭串口
        serial->clear();
        serial->close();
        serial->deleteLater();

        //恢复设置使能
        ui->PortBox->setEnabled(true);
        ui->BaudBox->setEnabled(true);
        ui->BitBox->setEnabled(true);
        ui->ParityBox->setEnabled(true);
        ui->StopBox->setEnabled(true);
        ui->OpenSerialButton->setText(tr("打开串口"));

        emit send_upmachine_state(0);
        emit send_start_or_not(false);
    }

}
//读取接收到的信息


//int i=0;
void Home::ReadData()
{
    QDateTime curDateTime=QDateTime::currentDateTime();
    QString currientTime=curDateTime.toString("yyyy-MM-dd hh:mm:ss");
    QByteArray buf;
    QByteArray arr="";

    buf = serial->readAll();

    if(!buf.isEmpty())
    {
        arr=arr.append(buf);
    }
    //qDebug()<<"接收到的数据："<<arr.toHex();
    FRAME=f->LinkFram(arr.toHex());     //获取到剪切后的数据，下一步传输到主页面进行显示
    if(FRAME!=0){                       //判断一下连接后的是否为0
        FRAME=f->CheckCode(FRAME);      //校验数据是否
    }
    if(FRAME!=0){                           //判断一下校验后的是否为0
        emit frame_to_mainwindow(FRAME);    //在这里定义一个signal用于向主页面发送数据并用于显示
    }
    show_receive=arr.toHex();               //将多个帧放在一起显示在界面里
    ui->receive_textEdit->append(show_receive);
    buf.clear();

}
void Home::on_ClearData_clicked(){
    ui->receive_textEdit->clear();
    ui->send_textEdit->clear();
    emit send_operation_messgae(0,"已清空显示框内的接收和发送数据",Qt::blue);
}

//发送按钮槽函数
void Home::on_SendButton_clicked()
{
//    qDebug()<<"on_SendButton_clicked is called";
    if(ui->OpenSerialButton->text() == tr("关闭串口")){
        serial->write(ui->send_textEdit->toPlainText().toLatin1());
    }else{
//        qDebug()<<"串口关闭";
    }
}

