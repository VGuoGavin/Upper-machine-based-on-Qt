#include "QMap"
#include "showpanel.h"
#include "ui_showpanel.h"
#include "dev/qcustomplot.h"
#include "four_pages/ctrpanel.h"

QMap<int,QString>all_list_map;      //存入所有项的键值对供查找
QMap<int,QString>chosed_list_map;   //被选中的复选框
QMap<int,QString>showgraph_list_map;//用于显示所选图像的map，int代表序号，bool代表是否被选中
//需要再定义一个Map，用于辨别showpanel页面复选框被选中的情况
//默认添加到showpanel的都是没有被选中的，这样方便处理和显示
//在showpanel页面选择后会添加graph显示对应的图像
QMap<int,QString>color;
//定义一个Widgets类供后面添加checkbox与lineedit用
Widgets::Widgets(QWidget *parent):QWidget(parent)
{
    color.insert(1,"black");
    color.insert(2,"red");
    color.insert(3,"blue");
    //this->resize(232, 27);
    formLayout = new QFormLayout(this);
    formLayout->setObjectName(QStringLiteral("formLayout"));
    formLayout->setHorizontalSpacing(0);
    formLayout->setVerticalSpacing(2);
    formLayout->setContentsMargins(2, 0, 0, 0);
    checkBox = new QCheckBox(this);
    checkBox->setObjectName(QStringLiteral("checkBox"));

    formLayout->setWidget(0, QFormLayout::LabelRole, checkBox);

    lineEdit = new QLineEdit(this);
    lineEdit->setObjectName(QStringLiteral("lineEdit"));
    lineEdit->setReadOnly(true);

    formLayout->setWidget(0, QFormLayout::FieldRole, lineEdit);

    checkBox->setMinimumWidth(200);
    checkBox->setMaximumWidth(200);
    //checkBox->setChecked(true);
    lineEdit->setMinimumWidth(50);

}


class Ctrpanel;
Showpanel::Showpanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Showpanel)
{
    ui->setupUi(this);
    Showpanel::plot();
    /*
    QString Rotate;
    QString Torque;
    QString Voltage;
    QString ;
    QString Sys_Station;
    QString Machine_Tempture;
    QString Controller_Tempture;
    QString Machine_Zeropoint;
    QString Machine_Tempturypoint;
    QString Max_AllowedTorque;
    QString Malfunction_Output;
    */
    all_list_map.insert(1,"电机转速");
    all_list_map.insert(2,"电机扭矩");
    all_list_map.insert(3,"母线电压");
    all_list_map.insert(4,"母线电流");
    all_list_map.insert(5,"系统状态");
    all_list_map.insert(6,"电机温度");
    all_list_map.insert(7,"控制器温度");
    all_list_map.insert(8,"电机零位角度");
    //delete _ctr;
}

Showpanel::~Showpanel()
{
    delete ui;
}

void Showpanel::plot(){

    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);//日期做X轴
    dateTicker->setDateTimeFormat("hh:mm:ss.zzz");      //日期格式(可参考QDateTime::fromString()函数)
    ui->customwidget->xAxis->setTicker(dateTicker);     //设置X轴为时间轴
    ui->customwidget->xAxis->setTickLabels(true);       //显示刻度标签
//    ui->customwidget->addGraph(ui->customwidget->xAxis, ui->customwidget->yAxis);

    ui->customwidget->setInteractions(QCP::iRangeDrag       //可平移
                                      | QCP::iRangeZoom     //可滚轮缩放
                                      | QCP::iSelectLegend  //可选中图例
                                      | QCP::iSelectAxes ); //可以选中横纵坐标放大缩小、
    ui->customwidget->legend->setVisible(true);

    //    ui->customwidget->legend->setFont(QFont("first"));
    //    ui->customwidget->yAxis->setRange(-1.5, 1.5);
    //    ui->customwidget->xAxis-> ->setRange(Qt::Horizontal);

    refreshTimer = startTimer(30, Qt::CoarseTimer);         //设置刷新频率
    sampleTimer = startTimer(500, Qt::CoarseTimer);         //设置采样频率

    tLast = getNow();

    lastPoint.setX(getNow());

}

//接收到帧数据，并实时更新ListWidget里的数据
int count=0;
void Showpanel::receive_frame(QString frame){
    QMap<int, QString>::iterator iter = chosed_list_map.begin();
    while (iter != chosed_list_map.end()){                           //遍历choose_list_map里的数据，只更新选中的项
        QList<QListWidgetItem*>item3 = ui->listWidget->findItems(iter.value(),Qt::MatchContains);
        if(!item3.isEmpty()){
            Widgets *itemWdt2 = static_cast<Widgets*>(ui->listWidget->itemWidget(item3.at(0)));
            itemWdt2->lineEdit->setText(frame.mid(iter.key()+10,20));
        }
        iter++;
    }

    /*下面是画动态图，这部分也非常重要*/
    QMap<int, QString>::iterator iter2 = showgraph_list_map.begin();
    int len=showgraph_list_map.count();
    int i=0;
    while (iter2 != showgraph_list_map.end()){  //遍历showgraph_list_map里的数据，只更新选中的项

        double xHigh = getNow() - 0.5;
        ui->customwidget->xAxis->setRange(xHigh - ui->customwidget->xAxis->range().size(), xHigh);

        ui->customwidget->replot();

        newPoint.setX(getNow());
        newPoint.setY((frame.mid(iter2.key()*2+12,2)).toFloat());     //接收到的数据
        qDebug() << newPoint.x() <<  newPoint.y();
        switch (i) {
        case 0:
            ui->customwidget->graph(i)->addData(newPoint.x(), newPoint.y()); //添加值
            ui->customwidget->graph(i)->setName(iter2.value());  //graph的名字与color对应
            ui->customwidget->graph(i)->setPen(QColor(color.find(iter2.key()).value()));
            break;
        case 1:
            ui->customwidget->graph(i)->addData(newPoint.x(), newPoint.y()); //添加值
            ui->customwidget->graph(i)->setName(iter2.value());  //graph的名字与color对应
            ui->customwidget->graph(i)->setPen(QColor(color.find(iter2.key()).value()));
            break;
        case 2:
            ui->customwidget->graph(i)->addData(newPoint.x(), newPoint.y()); //添加值
            ui->customwidget->graph(i)->setName(iter2.value());  //graph的名字与color对应
            ui->customwidget->graph(i)->setPen(QColor(color.find(iter2.key()).value()));
            break;
        default:
            break;
        }
        lastPoint.setX(newPoint.x());
        lastPoint.setY(newPoint.y());
        ui->customwidget->replot();
        i++;

        iter2++;
    }

}

void Showpanel::selected(QString data,bool action,int Number){
    if(action){
        /*下面这部分代码理清思绪，明白操作原理*/
        QListWidgetItem *item = new QListWidgetItem(data);
        item->setTextColor(Qt::white);
        Widgets *itemWdt = new Widgets(this);
        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, itemWdt);
        itemWdt->checkBox->setText(data);
        itemWdt->lineEdit->setText("NULL");
        //showpanel界面的复选框状态改变
        connect(itemWdt->checkBox, SIGNAL(clicked()), this, SLOT(change_graph()));
        //获取到指定的item并修改数值
        chosed_list_map.insert(Number,all_list_map.find(Number).value());

    }else{                                       //取消勾选直接删掉对应的item与widget
        QList<QListWidgetItem*> item2;
        item2=ui->listWidget->findItems(data,Qt::MatchExactly);
        //        qDebug()<<"要删除的item"<<item2;
        if(!item2.isEmpty()){
            delete item2.at(0);
            ui->listWidget->removeItemWidget(item2.at(0));
        }
        chosed_list_map.remove(Number);         //同时更新chosed_list_map的内容
    }
}

double Showpanel::getNow(){
    return (double)(QDateTime::currentMSecsSinceEpoch()) / 1000.0;
}
void Showpanel::change_graph(){
    qDebug()<<"change_graph";
    QMap<int, QString>::iterator iter = chosed_list_map.begin();
    showgraph_list_map.clear();                 //这里是遍历的choose_list_map并添加数据，可以改成逐个添加和清除
    ui->customwidget->clearGraphs();
    while (iter != chosed_list_map.end()){      //遍历choose_list_map里的数据，只更新选中的项
        QList<QListWidgetItem*>item3 = ui->listWidget->findItems(iter.value(),Qt::MatchContains);
        if(!item3.isEmpty()){
            Widgets *itemWdt2 = static_cast<Widgets*>(ui->listWidget->itemWidget(item3.at(0)));
            if(itemWdt2->checkBox->checkState()){
                //每次按钮变动都更新一下选中的list，因为是全局map上面receive_frame内自动根据该map更新
                qDebug()<<"update_graph"<<itemWdt2->checkBox->text();
                showgraph_list_map.insert(iter.key(),iter.value());
                ui->customwidget->addGraph(ui->customwidget->xAxis, ui->customwidget->yAxis);
                //update_graph(iter.key());
            }
        }
        iter++;
    }
}

