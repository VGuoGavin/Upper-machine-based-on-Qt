#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include "four_pages/home.h"
#include "four_pages/ctrpanel.h"
#include "four_pages/showpanel.h"
#include "four_pages/calibration.h"
#include <QStackedWidget>
#include <QListWidget>

#define OBJ_SYS         0x01   //系统信息
#define OBJ_SEND        0x02   //发送信息
#define OBJ_RECV        0x03   //接收信息
#define OBJ_ERR         0x04   //错误信息
#define OBJ_REDO        0x05   //重发信息
#define OBJ_DEVI        0x06   //采集设备
#define OBJ_DEVII       0x07   //控制器设备

namespace Ui {                          //Ui是指向Ui界面的指针，对应于home.ui文件
class MainWindow;                       //指定Ui作用域，作用域此类里
}

class MainWindow : public QMainWindow   //继承于Qwidget
{
    Q_OBJECT                            //Q_OBJECT间接子类，拥有很多重要的功能，比如信号与槽机制，必须添加

public:
    explicit MainWindow(QWidget *parent = 0); //explicit(明确的)防止隐式转换
    Home *homewidget;
    Ctrpanel *ctrwidget;
    Calibration *calibration;
    QString stringToHtml(QString, QColor);

    ~MainWindow();                           //析构函数

signals:                                     //信号函数，页面内产生信号的函数
    void data_to_showpanel(QString,bool,int);//将controlpanel中选择复选框数据发送给showpanel
    void frame_to_ctrpanel(QString);         //向control panel发送帧数据
    void frame_to_showpanel(QString);        //向showpanel页面发送帧数据
    void save_operation_message(QString);

private slots:                               //这里定义的是槽函数，对应的信号来后出发下面的函数
    void ChangetoHome();                     //切换到对应listwidget页面
    void ChangetoCtrpanel();
    void ChangetoShowpanel();
    void ChangetoCalibration();

    void Open_file();
    void Save_as();

    void receive_option(QString,bool,int);        //从Control panel页面传来的复选框数据
    //void pushButton4();
    void receive_frame(QString);
    void operation_message(int,QString,QColor);

    void upmachine_message(int);

    void on_pushButton_4_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton4_clicked();

private:
    Ui::MainWindow *ui;
    QStackedWidget *pages;


    Showpanel *showwidget;
    
    QAction *open_file;
    QAction *save_as;


};

#endif // MAINWINDOW_H
