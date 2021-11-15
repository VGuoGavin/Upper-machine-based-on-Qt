#ifndef HOME_H
#define HOME_H

#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

namespace Ui {                          //Ui是指向Ui界面的指针，对应于home.ui文件
class Home;                             //指定Ui作用域，作用域Home类里
}

class Home : public QWidget             //Home继承于Qwidget
{
    Q_OBJECT                            //Q_OBJECT间接子类，拥有很多重要的功能，比如信号与槽机制，必须添加

public:

    explicit Home(QWidget *parent = 0); //explicit(明确的)防止隐式转换
    Ui::Home *ui;                       //修改成public在其他界面可以操纵本页的控件

    void ReadData();
//    void on_SendButton_clicked();
    void OnActionTest();

    QSerialPort *serial;
    QString test1=0;
    QString show_receive="";
    void Find_Port();
    ~Home();

signals:
    void frame_to_mainwindow(QString);
    void send_operation_messgae(int,QString,QColor);
    void send_upmachine_state(int);
    void send_start_or_not(bool);

private slots:

   void on_OpenSerialButton_clicked();
   void on_SendButton_clicked();
   void on_ClearData_clicked();
   void on_FindPort_clicked();

private:


};

#endif // HOME_H
