#include "ctrpanel.h"
#include "ui_ctrpanel.h"
#include <QDir>
#include <QDateTime>
#include <QDebug>
#include "ui_showpanel.h"

//结构体预留备用
typedef struct _Message_{
    QString Rotate;
    QString Torque;
    QString Voltage;
    QString Electricity;
    QString Sys_Station;
    QString Machine_Tempture;
    QString Controller_Tempture;
    QString Machine_Zeropoint;
    QString Machine_Tempturypoint;
    QString Max_AllowedTorque;
    QString Malfunction_Output;
}MESSAGE;


Ctrpanel::Ctrpanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Ctrpanel)
{
    ui->setupUi(this);
    //向下位机发送数据
    connect(ui->sendButton,SIGNAL(clicked()),this, SLOT(trigger_select()));

}

Ctrpanel::~Ctrpanel()
{
    delete ui;
}
//将接收到的帧通过截取直接显示在页面上
void Ctrpanel::receive_frame(QString frame){
    MESSAGE Message;
    Frame=frame.toUtf8();
    Message.Rotate          =    frame.mid(4,2);
    Message.Voltage         =    frame.mid(6,2);
    Message.Electricity     =    frame.mid(8,2);
    Message.Machine_Tempture=    frame.mid(10,2);
    Message.Controller_Tempture= frame.mid(12,2);
    Message.Machine_Zeropoint=   frame.mid(14,2);
    Message.Machine_Tempturypoint=frame.mid(16,2);
    Message.Max_AllowedTorque=   frame.mid(18,2);
    Message.Malfunction_Output=  frame.mid(20,2);

    ui->mcu_ox00_lineEdit->setText(Message.Rotate);
    ui->mcu_ox01_lineEdit->setText(Message.Voltage);
    ui->mcu_ox02_lineEdit->setText(Message.Electricity);
    ui->mcu_ox03_lineEdit->setText(Message.Machine_Tempture);
    ui->mcu_ox04_lineEdit->setText(Message.Controller_Tempture);
    ui->mcu_ox05_lineEdit->setText(Message.Machine_Zeropoint);
    ui->mcu_ox06_lineEdit->setText(Message.Machine_Tempturypoint);
    ui->mcu_ox07_lineEdit->setText(Message.Max_AllowedTorque);
    ui->mcu_ox08_lineEdit->setText(Message.Malfunction_Output);
    //qDebug()<<"控制台接收到帧数据"<<frame;
}

//生成操作日志
void Ctrpanel:: save_operationblog(QString blog){
    QDateTime curDateTime=QDateTime::currentDateTime();
    QString currientTime=curDateTime.toString("yyyy-MM-dd hh:mm:ss");
    QString blog_name=curDateTime.toString("yyyy-MM-dd");
    //    qDebug()<<"currientTime"<<currientTime;
    //    QString cur_path=QDir::currentPath();
    //    QString filepath=cur_path+"blog.txt";
    //    qDebug()<<"file_path:"<<filepath;
    QString dir="D:/GoBao1_operation_Blog/";
    QString file_name = blog_name+".txt";   //日志的名字为当天的日期

    QString file_path=dir+file_name;        //具体路径为文件夹加文件名
    QFile *file= new QFile(file_path);

    QDir *Dir=new QDir();

    if(!Dir->exists(dir)){
        Dir->mkdir(dir);        //找不到就直接生成对应文件夹
        emit send_operation_messgae(0,"生成文件，见D:/GoBao1_operation_Blog/",Qt::blue);
    }
    delete Dir;
    if(!file->exists(file_path)){
        file->open(QIODevice::WriteOnly);
        file->close();          //不存在对应的文件就直接生成
        emit send_operation_messgae(0,"生成Blog文件，见D:/GoBao1_operation_Blog/下",Qt::blue);
    }
    if(!file->open(QIODevice::ReadWrite | QIODevice::Text)) {
        QMessageBox::critical(this, tr("QMessageBox::critical()"),
                              "无法打开Blog文件！",
                              QMessageBox::Retry | QMessageBox::Ignore);
        emit send_operation_messgae(0,"无法打开Blog文件",Qt::red);
    }else{
        //写数据
        emit send_operation_messgae(0,"成功打开Blog文件",Qt::blue);
        QString all=file->readAll();
        //qDebug()<<"all"<<all;
        //QByteArray wdata(currientTime.toLatin1());
        //qDebug()<<"currientTime.toLatin1()"<<wdata;
        file->write(all.toUtf8()+blog.toUtf8());
        file->close();
        delete file;

        QMessageBox::information(this, tr("操作提醒！"),
                              "成功保存到"+file_path,
                              QMessageBox::Ok);
    }
}

//void Ctrpanel:: receive_filepath(QString receive_path,bool rceive_controller){
//    path=receive_path;
//    controller=rceive_controller;
//}

//自动保存帧数据，并没用到
/*
int cnt=0;
QString path="";
bool controller=false;
void Ctrpanel::savedata(){
    if(controller==true){
    if(cnt>50){
        QDateTime curDateTime=QDateTime::currentDateTime();
//        QString currientTime=curDateTime.toString("yyyy-MM-dd hh:mm:ss");
//        QString blog_name= curDateTime.toString("yyyy-MM-dd");
        QString time=curDateTime.toString("hh:mm:ss");
//        qDebug()<<"currientTime"<<currientTime;

//        QString dir="D:/Qt program/GoBao1/Blog/";
//        QString file_name = blog_name+".csv";
//        qDebug()<<"file_name:"<<file_name;
//        QString file_path=dir+file_name;

        QFile *file= new QFile(path);
        qDebug()<<"file_path:"<<path;

        //这里是生成一个文件夹
//        QDir *Dir=new QDir();
//        if(!Dir->exists(path)){
//            Dir->mkdir(dir);
//        }
//        delete Dir;
        if(!file->exists(path)){
            file->open(QIODevice::WriteOnly);
            file->close();
        }
        qDebug() <<"successful find Blog";
        if(!file->open(QIODevice::ReadWrite | QIODevice::Text)) {
            qDebug()<<"Can't open the file!"<<endl;
        }else{
            //写数据
            QString all=file->readAll();
            qDebug()<<"all"<<all;
            QByteArray wdata(time.toLatin1());
            qDebug()<<"currientTime.toLatin1()"<<wdata;
            file->write(wdata+","+Frame+"\n");
            file->close();
            delete file;
        }
        cnt=0;
    }else{
        cnt++;
    }
    }else{
     qDebug()<<"没有保存"   ;
    }
}
*/
//下面都是复选框选中后触发的发送信号
void Ctrpanel::on_mcu_ox00_checkbox_clicked(bool checked)
{
    QString str=ui->mcu_ox00_checkbox->text();
    int number=1;
    emit send_option(str,checked,number); //第1个选中
    operation(str,checked);
}

void Ctrpanel::on_mcu_ox01_checkBox_clicked(bool checked)
{
    QString str=ui->mcu_ox01_checkBox->text();
    int number=2;
    emit send_option(str,checked,number);    //第2个选中
    operation(str,checked);
}

void Ctrpanel::on_mcu_ox02_checkbox_clicked(bool checked)
{
    QString str=ui->mcu_ox02_checkbox->text();
    int number=3;
    emit send_option(str,checked,number);
    operation(str,checked);
}
void Ctrpanel::operation(QString str, bool checked){
    if(checked){
        emit send_operation_messgae(0,"选中"+str,Qt::blue);
    }else{
        emit send_operation_messgae(0,"取消选中"+str,Qt::blue);
    }
}

void Ctrpanel::on_sendButton_clicked()
{
    qDebug()<<"ui->comboBox_3->currentIndex():"<<ui->comboBox_3->currentIndex();
}



