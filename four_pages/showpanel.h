#ifndef SHOWPANEL_H
#define SHOWPANEL_H

#include <QWidget>
#include "dev/qcustomplot.h"
#include "ctrpanel.h"
#include "ui_ctrpanel.h"
#include "dev/frame.h"
#include <QPointF>
#include <QDateTime>

namespace Ui {
class Showpanel;
}

class Widgets: public QWidget
{
    Q_OBJECT
public:
    explicit Widgets(QWidget *parent = 0);
    QCheckBox *checkBox;
    QLineEdit *lineEdit;

private:
    QFormLayout *formLayout;

};

class Showpanel : public QWidget
{
    Q_OBJECT

public:
    explicit Showpanel(QWidget *parent = 0);
    void plot();
    void savedata(QString);
    ~Showpanel();
private:
    //画图专用
    double getNow();
    int refreshTimer;//刷新图像的定时器
    int sampleTimer;//模拟采样的定时器
    double tLast;
    QPointF newPoint;
    QPointF lastPoint;
    int cnt;
protected:
//    void timerEvent(QTimerEvent *event);

signals:
//    void send_savedata_signal(QString,bool);
    void send_operation_messgae(int,QString,QColor);

private slots:
    void selected(QString data,bool action,int Number);        //负责接收来自control panel页面选择的结果
    void receive_frame(QString);                               //从mainwindow接收到的帧数据
    //void canceled(QString No);
    void change_graph();
    void on_Save_Button_clicked(bool checked);
    void receive_start_or_not(bool);                //判断是否已经开启UART数据通信

private:
    Ui::Showpanel *ui;
    QMap<QListWidgetItem*, QCheckBox*> map;                   //item映射到CheckBox
    void initial();

};

#endif // SHOWPANEL_H
