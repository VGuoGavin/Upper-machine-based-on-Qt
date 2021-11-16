#include "ctrpanel.h"
#include "ui_ctrpanel.h"
#include <QDir>
#include <QDateTime>
#include <QDebug>
#include "ui_showpanel.h"
#include <vector>

float all_list[27];     //定义出所有的数据样本


Ctrpanel::Ctrpanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Ctrpanel)
{
    ui->setupUi(this);
    //向下位机发送数据
    connect(ui->sendButton,SIGNAL(clicked()),this, SLOT(trigger_select()));
    for(int i=0;i<27;i++){
        all_list[i]=0;                   //list_map赋初值
    }
}

Ctrpanel::~Ctrpanel()
{
    delete ui;
}
//将接收到的帧通过截取直接显示在页面上
//已经修改了指定位置截取帧，符合协议，只需要再略微修改即可。
void Ctrpanel::receive_frame(QString frame){            /*这个接收到的是系列号、地址、命令为  */
    bool ok;
    int preface=60;
    //    int length=frame.mid(10,2).toInt(&ok,16);
    //    int begin=frame.mid(12,4).toInt(&ok,16);
    int length=30;
    int begin=0;
//    qDebug()<<"frame"<<frame;
    for(int i=0; i<length;i++){
        if(begin+i<24){
            all_list[begin+i]=frame.mid(preface+begin+i,2).toInt(&ok,16);
//            qDebug()<<"all_list"<<all_list[begin+i];
        }else{
            all_list[begin+i]=frame.mid(preface+begin+i,4).toInt(&ok,16);
        }
    }
//    qDebug()<<"all_list"<<all_list;
    ui->mcu_ox00_lineEdit->setText(QString::number(all_list[0],'f',2));
    ui->mcu_ox01_lineEdit->setText(QString::number(all_list[1],'f',2));
    ui->mcu_ox02_lineEdit->setText(QString::number(all_list[2],'f',2));
    ui->mcu_ox03_lineEdit->setText(QString::number(all_list[3],'f',2));
    ui->mcu_ox04_lineEdit->setText(QString::number(all_list[4],'f',2));
    ui->mcu_ox05_lineEdit->setText(QString::number(all_list[5],'g',10));
    ui->mcu_ox06_lineEdit->setText(QString::number(all_list[6],'g',10));
    ui->mcu_ox07_lineEdit->setText(QString::number(all_list[7],'g',10));
    ui->mcu_ox08_lineEdit->setText(QString::number(all_list[8],'g',10));
    ui->mcu_ox09_lineEdit->setText(QString::number(all_list[9],'g',10));
    ui->mcu_ox0A_lineEdit->setText(QString::number(all_list[10],'g',10));
    ui->mcu_ox0B_lineEdit->setText(QString::number(all_list[11],'f',2));
    ui->mcu_ox0C_lineEdit->setText(QString::number(all_list[12],'f',2));
    ui->mcu_ox0D_lineEdit->setText(QString::number(all_list[13],'f',2));
    ui->mcu_ox0E_lineEdit->setText(QString::number(all_list[14],'f',2));
    ui->mcu_ox0F_lineEdit->setText(QString::number(all_list[15],'f',2));
    ui->mcu_ox10_lineEdit->setText(QString::number(all_list[16],'g',10));
    ui->mcu_ox11_lineEdit->setText(QString::number(all_list[17],'g',10));
    ui->mcu_ox12_lineEdit->setText(QString::number(all_list[18],'g',10));
    ui->mcu_ox13_lineEdit->setText(QString::number(all_list[19],'g',10));
    ui->mcu_ox14_lineEdit->setText(QString::number(all_list[20],'g',10));
    ui->mcu_ox15_lineEdit->setText(QString::number(all_list[21],'g',10));
    ui->mcu_ox16_lineEdit->setText(QString::number(all_list[22],'g',10));
    ui->mcu_ox17_lineEdit->setText(QString::number(all_list[23],'g',10));
    ui->mcu_ox18_lineEdit->setText(QString::number(all_list[24],'g',10));
    ui->mcu_ox19_lineEdit->setText(QString::number(all_list[25],'g',10));
    ui->mcu_ox1A_lineEdit->setText(QString::number(all_list[26],'g',10));

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


/*下边是发送数据所进行的格式化*/
void Ctrpanel::on_sendButton_clicked()
{
    //将Int形转为16进制并在前面补零
    QString speed_0x00  =   QString("%1").arg(ui->set_0x00->text().toInt(),2,16,QLatin1Char('0'));
    QString torque_0x01 =   QString("%1").arg((int)(ui->set_0x01->text().toFloat()*100),2,16,QLatin1Char('0')) ;
    QString Id_0x02     =   QString("%1").arg(ui->set_0x02->text().toInt(),2,16,QLatin1Char('0'));
    QString Iq_0x03     =   QString("%1").arg(ui->set_0x03->text().toInt(),2,16,QLatin1Char('0'));
    QString Vd_0x04     =   QString("%1").arg(ui->set_0x04->text().toInt(),2,16,QLatin1Char('0'));
    QString Vq_0x05     =   QString("%1").arg(ui->set_0x05->text().toInt(),2,16,QLatin1Char('0'));
    QString mode_0x06_1 =   QString("%1").arg(ui->set_0x06_1->currentIndex(),2,16,QLatin1Char('0'));
    QString mode_0x06_2 =   QString("%1").arg(ui->set_0x06_2->currentIndex(),2,16,QLatin1Char('0'));

    QString frame=speed_0x00+torque_0x01+Id_0x02+Iq_0x03+Vd_0x04+Vq_0x05+mode_0x06_1+mode_0x06_2;
//    qDebug()<<"frame:"<<frame;
    emit send_frame(frame);

}


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

//向操作日志填写内容
void Ctrpanel::operation(QString str, bool checked){
    if(checked){
        emit send_operation_messgae(0,"选中"+str,Qt::blue);
    }else{
        emit send_operation_messgae(0,"取消选中"+str,Qt::blue);
    }
}

//下面都是复选框选中后触发的发送信号
void Ctrpanel::on_mcu_ox00_checkbox_clicked(bool checked)
{
    QString str=ui->mcu_ox00_checkbox->text();
    int number=0;
    emit send_option(str,checked,number); //第1个选中
    operation(str,checked);
}

void Ctrpanel::on_mcu_ox01_checkBox_clicked(bool checked)
{
    QString str=ui->mcu_ox01_checkBox->text();
    int number=1;
    emit send_option(str,checked,number);    //第2个选中
    operation(str,checked);
}

void Ctrpanel::on_mcu_ox02_checkbox_clicked(bool checked)
{
    QString str=ui->mcu_ox02_checkbox->text();
    int number=2;  emit send_option(str,checked,number);  operation(str,checked);
}

void Ctrpanel::on_mcu_ox03_checkbox_clicked(bool checked)
{
    QString str=ui->mcu_ox03_checkbox->text();
    int number=3;  emit send_option(str,checked,number);  operation(str,checked);
}

void Ctrpanel::on_mcu_ox04_checkbox_clicked(bool checked)
{
    QString str=ui->mcu_ox04_checkbox->text();
    int number=4;  emit send_option(str,checked,number);  operation(str,checked);
}

void Ctrpanel::on_mcu_ox05_checkbox_clicked(bool checked)
{
    QString str=ui->mcu_ox05_checkbox->text();
    int number=5;  emit send_option(str,checked,number);  operation(str,checked);
}

void Ctrpanel::on_mcu_ox06_checkbox_clicked(bool checked)
{
    QString str=ui->mcu_ox06_checkbox->text();
    int number=6;  emit send_option(str,checked,number);  operation(str,checked);
}

void Ctrpanel::on_mcu_ox07_checkbox_clicked(bool checked)
{
    QString str=ui->mcu_ox07_checkbox->text();
    int number=7;  emit send_option(str,checked,number);  operation(str,checked);
}

void Ctrpanel::on_mcu_ox08_checkbox_clicked(bool checked)
{
    QString str=ui->mcu_ox08_checkbox->text();
    int number=8;  emit send_option(str,checked,number);  operation(str,checked);
}

void Ctrpanel::on_mcu_ox09_checkbox_clicked(bool checked)
{
    QString str=ui->mcu_ox09_checkbox->text();
    int number=9;  emit send_option(str,checked,number);  operation(str,checked);
}

void Ctrpanel::on_mcu_ox0A_checkbox_clicked(bool checked)
{
    QString str=ui->mcu_ox0A_checkbox->text();
    int number=10;  emit send_option(str,checked,number);  operation(str,checked);
}

void Ctrpanel::on_mcu_ox0B_checkbox_clicked(bool checked)
{
    QString str=ui->mcu_ox0B_checkbox->text();
    int number=11;  emit send_option(str,checked,number);  operation(str,checked);
}

void Ctrpanel::on_mcu_ox0C_checkbox_clicked(bool checked)
{
    QString str=ui->mcu_ox0C_checkbox->text();
    int number=12;  emit send_option(str,checked,number);  operation(str,checked);
}

void Ctrpanel::on_mcu_ox0D_checkbox_clicked(bool checked)
{
    QString str=ui->mcu_ox0D_checkbox->text();
    int number=13;  emit send_option(str,checked,number);  operation(str,checked);
}

void Ctrpanel::on_mcu_ox0E_checkbox_clicked(bool checked)
{
    QString str=ui->mcu_ox0E_checkbox->text();
    int number=14;  emit send_option(str,checked,number);  operation(str,checked);
}

void Ctrpanel::on_mcu_ox0F_checkbox_clicked(bool checked)
{
    QString str=ui->mcu_ox0F_checkbox->text();
    int number=15;  emit send_option(str,checked,number);  operation(str,checked);
}

void Ctrpanel::on_mcu_ox10_checkbox_clicked(bool checked)
{
    QString str=ui->mcu_ox10_checkbox->text();
    int number=16;  emit send_option(str,checked,number);  operation(str,checked);
}

void Ctrpanel::on_mcu_ox11_checkbox_clicked(bool checked)
{
    QString str=ui->mcu_ox11_checkbox->text();
    int number=17;  emit send_option(str,checked,number);  operation(str,checked);
}

void Ctrpanel::on_mcu_ox12_checkbox_clicked(bool checked)
{
    QString str=ui->mcu_ox12_checkbox->text();
    int number=18;  emit send_option(str,checked,number);  operation(str,checked);
}

void Ctrpanel::on_mcu_ox13_checkbox_clicked(bool checked)
{
    QString str=ui->mcu_ox13_checkbox->text();
    int number=19;  emit send_option(str,checked,number);  operation(str,checked);
}

void Ctrpanel::on_mcu_ox14_checkbox_clicked(bool checked)
{
    QString str=ui->mcu_ox14_checkbox->text();
    int number=20;  emit send_option(str,checked,number);  operation(str,checked);
}

void Ctrpanel::on_mcu_ox15_checkbox_clicked(bool checked)
{
    QString str=ui->mcu_ox15_checkbox->text();
    int number=21;  emit send_option(str,checked,number);  operation(str,checked);
}

void Ctrpanel::on_mcu_ox16_checkbox_clicked(bool checked)
{
    QString str=ui->mcu_ox16_checkbox->text();
    int number=22;  emit send_option(str,checked,number);  operation(str,checked);
}

void Ctrpanel::on_mcu_ox17_checkbox_clicked(bool checked)
{
    QString str=ui->mcu_ox17_checkbox->text();
    int number=23;  emit send_option(str,checked,number);  operation(str,checked);
}

void Ctrpanel::on_mcu_ox18_checkbox_clicked(bool checked)
{
    QString str=ui->mcu_ox18_checkbox->text();
    int number=24;  emit send_option(str,checked,number);  operation(str,checked);
}

void Ctrpanel::on_mcu_ox19_checkbox_clicked(bool checked)
{
    QString str=ui->mcu_ox19_checkbox->text();
    int number=25;  emit send_option(str,checked,number);  operation(str,checked);
}

void Ctrpanel::on_mcu_ox1A_checkbox_clicked(bool checked)
{
    QString str=ui->mcu_ox1A_checkbox->text();
    int number=26;  emit send_option(str,checked,number);  operation(str,checked);
}

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
