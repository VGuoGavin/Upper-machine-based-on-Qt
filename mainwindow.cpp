#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_home.h"
#include "ui_ctrpanel.h"
#include "ui_calibration.h"
#include <QDebug>
#include <Qmenu>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QTextEdit>
#include <QDateTime>
#include "dev/frame.h"

#define LOG_COLOR               (Qt::blue)
#define LOG_ERR                 (Qt::red)

QString currentFilePath="";
extern Home *p_home=nullptr;
extern Showpanel *p_show=nullptr;
extern Ctrpanel *p_ctrpanel=nullptr;
//extern Frame *f=new Frame();
QString currientTime="";


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("GoBao control Panel");
    setWindowIcon(QIcon("../GoBao1/Images/communacation2.png"));

    Home *homewidget=new Home(this);              //三个页面
    p_home=homewidget;

    Ctrpanel *ctrwidget=new Ctrpanel(this);
    p_ctrpanel=ctrwidget;

    Showpanel *showwidget=new Showpanel(this);
    p_show=showwidget;


    Calibration *calibration=new Calibration(this);

    QAction *open_file= new QAction("打开文件"); //添加动作，动作会自动与被连接控件匹配
    QAction *save_as=  new QAction("另存为");
    QAction *help=new QAction("帮助");
    QAction *about=new QAction("关于");

    QMenu *File=new QMenu("文件");          //创建菜单栏
    QMenu *Setting=new QMenu("设置");
    QMenu *Help=new QMenu("帮助");
    QMenu *About=new QMenu("关于");

    ui->menuBar->addMenu(File);             //添加菜单栏
    ui->menuBar->addMenu(Setting);
    ui->menuBar->addMenu(Help);
    ui->menuBar->addMenu(About);

    File->addAction(open_file);             //创建menu并与action连接
    File->addAction(save_as);
    Help->addAction(help);
    About->addAction(about);

    //QMenu *Save=new QMenu("Save as");     //另一种常见menu的方式
    //File->addMenu(Save);                  //直接添加菜单

    ui->stackedWidget->addWidget(homewidget);
    ui->stackedWidget->addWidget(ctrwidget);
    ui->stackedWidget->addWidget(showwidget);
    ui->stackedWidget->setCurrentWidget(homewidget);
    ui->stackedWidget->addWidget(calibration);

    connect(open_file,&QAction::triggered,this,&MainWindow::Open_file);      //将动作与信号相连
    connect(save_as,&QAction::triggered,this,&MainWindow::Save_as);
    connect(help,&QAction::triggered,this,&MainWindow::Help);
    connect(about,&QAction::triggered,this,&MainWindow::About);

    QDateTime curDateTime=QDateTime::currentDateTime();
    currientTime=curDateTime.toString("yyyy-MM-dd hh:mm:ss");
    qDebug()<<"currientTime"<<currientTime;

    //主界面接收control panel 选择后的数据
    connect(ctrwidget,SIGNAL(send_option(QString,bool,int)),this, SLOT(receive_option(QString,bool,int)));
    //将controlpanel中选择复选框数据发送给showpanel
    QObject::connect(this,SIGNAL(data_to_showpanel(QString,bool,int)), p_show,SLOT(selected(QString,bool,int)));

    //home页面获得的截取后数据传过来再用于显示
    connect(p_home,SIGNAL(frame_to_mainwindow(QString)),this,SLOT(receive_frame(QString)));
    connect(this,SIGNAL(frame_to_ctrpanel(QString)),p_ctrpanel,SLOT(receive_frame(QString)));
    connect(this,SIGNAL(frame_to_showpanel(QString)),p_show,SLOT(receive_frame(QString)));

    //这里是在信息框输出对应的操作信息与提示
    connect(p_home,SIGNAL(send_operation_messgae(int,QString,QColor)),this,SLOT(operation_message(int,QString,QColor)));
    connect(p_ctrpanel,SIGNAL(send_operation_messgae(int,QString,QColor)),this,SLOT(operation_message(int,QString,QColor)));
    connect(p_show,SIGNAL(send_operation_messgae(int,QString,QColor)),this,SLOT(operation_message(int,QString,QColor)));
    //关掉这个连接，直接在showpanel内实现更直接
    connect(p_home,SIGNAL(send_start_or_not(bool)),p_show,SLOT(receive_start_or_not(bool)));

    //将show页面出发保存数据的信号传到control页面
    connect(p_show,SIGNAL(send_savedata_signal(QString,bool)),p_ctrpanel,SLOT(receive_filepath(QString,bool)));
    //操作日志
    connect(this,SIGNAL(save_operation_message(QString)),p_ctrpanel,SLOT(save_operationblog(QString)));


    this->upmachine_message(0);
    //ctrwidget->Blog();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//传来的是在control panel界面选择的check box的信号
void MainWindow::receive_option(QString receive_option,bool action,int number){
    emit data_to_showpanel(receive_option,action,number);
    //return 0;
}

void MainWindow::receive_frame(QString data){
    if(data!=0){
        //成功拿到连接好的帧数据
        p_home->ui->board_0x00_lineEdit->setText(data);
        //将数据发送到control panel页面和show panel页面进行显示
        emit frame_to_ctrpanel(data);
        emit frame_to_showpanel(data);
    }
}



void MainWindow::Open_file()
{
    //获取文件绝对路径
    QString file_path = QFileDialog::getOpenFileName(this,"open file"," ",tr("CSV文件(*.csv)"));// tr("Allfile(*.*);;mp3file(*.txt)"));
    qDebug() <<"file path:"<<file_path;

    QFile *file= new QFile(file_path);

    if(file->exists()){
        QByteArray all;
        //QByteArray line;

        if(!file->open(QIODevice::ReadWrite | QIODevice::Text)) {
            qDebug()<<"Can't open the file!"<<endl;
        }else{
            /*
            while(!file->atEnd()) {
                line = file->readLine();         //循环一行一行的读
                qDebug()<< line;
            } */
            all=file->readAll();
            qDebug()<< all;
            p_home->ui->board_0x00_lineEdit->setText(all);  //测试操纵子页面的控件
            //写数据
            //QByteArray wdata("hello world\n");
            //file->write(wdata);

            file->close();
            delete file;
        }

        //ui->lineEdit->setText(line);
    }
    else{
        qDebug() <<"no file";
    }

}
void MainWindow::Save_as(){

    //将数据另存为
    QString save_file=QFileDialog::getSaveFileName(this, tr("Save As"), "", tr("Curve TagName Files (*.csv)"));
    qDebug()<<"save_file:"<<save_file;
    QFile *file2=new QFile(save_file);

    QTextEdit *CurrentContent= new QTextEdit(this);
    CurrentContent->setText("高标,科技");
    CurrentContent->document()->setModified(false);

    if (save_file==""){
        return;
    }else{
        currentFilePath=save_file;
        if(!file2->open(QIODevice::WriteOnly|QIODevice::Text)){
            qDebug()<<"error about save as path";
            return;
        }
        QTextStream textStream(file2);
        textStream<<CurrentContent->toPlainText();
        file2->close();
        delete file2;
    }

}
void MainWindow::operation_message(int code, QString text,QColor color){
    qDebug()<<"err code:"<<code<<"text:"<<text;
    QString debug_tmp="";
    QTime curtime = QTime::currentTime();

    //-------add current time-----------
    debug_tmp += "[" + curtime.toString("hh:mm:ss.z")+ "]";

    //--------add obj name---------------
    switch(code)
    {
        case 0:  debug_tmp += "[系统]";break;
        case 1:  debug_tmp += "[发送]";break;
        case 2:  debug_tmp += "[接收]";break;
        case 3:  debug_tmp += "[错误]";break;
        case 4:  debug_tmp += "[重发]";break;
        case 5:  debug_tmp += "[采集设备]";break;
        case 6: debug_tmp += "[控制器设备]";break;
        default:break;
    }

   //--------add the data of serial-------
   //recv_tmp += uCharToHexString(dat_tmp, i);
   //debug_tmp += ByteArrayToHexString(serial_dat);  //统一

   //-------add except text tips----------
    debug_tmp += text;

   //-------add line change symbol--------
    debug_tmp += "\n";

    //-----update logbrowser text--------
    ui->blog_textEdit->moveCursor(QTextCursor::End);
    debug_tmp = stringToHtml(debug_tmp, color);
    ui->blog_textEdit->append(debug_tmp);

}

QString MainWindow::stringToHtml(QString str, QColor crl)
{
    QByteArray array;
    array.append(crl.red());
    array.append(crl.green());
    array.append(crl.blue());
    QString strC(array.toHex());
    //return (QString("<span style=\" color:#%1;\">%2</span>").arg(strC).arg(str));
    return (QString("<font color=\"#%1\">%2</font>").arg(strC).arg(str));
}

void MainWindow:: upmachine_message(int res){
    QString str="";
    if(res){
        str=stringToHtml("已连接",Qt::red);
        ui->label->setText(str);
    }else{
        str=stringToHtml("空闲",Qt::blue);
        ui->label->setText(str);
    }
}


//这里可以用switch语句优化下，多个页面组合在一个函数里

void MainWindow::on_home_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->ctr_pushButton->setChecked(false);
    ui->show_pushButton->setChecked(false);
    ui->clibration_pushButton->setChecked(false);
}

void MainWindow::on_ctr_pushButton_clicked()
{
    int i=ui->stackedWidget->currentIndex();
     //    qDebug()<<"i:"<<i;
         ui->stackedWidget->setCurrentIndex(3);
    ui->home_pushButton->setChecked(false);
    ui->show_pushButton->setChecked(false);
    ui->clibration_pushButton->setChecked(false);
}


void MainWindow::on_show_pushButton_clicked()
{
    int j=ui->stackedWidget->currentIndex();
//    qDebug()<<"j:"<<j;
    ui->stackedWidget->setCurrentIndex(4);
    ui->home_pushButton->setChecked(false);
    ui->ctr_pushButton->setChecked(false);
    ui->clibration_pushButton->setChecked(false);
}

void MainWindow::on_clibration_pushButton_clicked()
{
    int j=ui->stackedWidget->currentIndex();
//    qDebug()<<"j:"<<j;
    ui->stackedWidget->setCurrentIndex(5);
    ui->home_pushButton->setChecked(false);
    ui->ctr_pushButton->setChecked(false);
    ui->show_pushButton->setChecked(false);
}

void MainWindow::Help(){
    QMessageBox::information(this,"帮助", "1、程序启动会自动搜索可用端口，如果查找失败可以手动操作，连接后可以收发数据。\n2、示波器界面保存的数据是图像列表中所有项的数据。\n3、控制下位机界面输入数据后回车便将数据放松到下位机。");
}
void MainWindow::About(){
    QMessageBox::information(this,"关于", "广东高标电子科技中控上位机。");
}


void MainWindow::on_save_pushButton_clicked()
{
    emit save_operation_message(ui->blog_textEdit->toPlainText());
}

void MainWindow::on_clearblog_pushButton_clicked()
{
     ui->blog_textEdit->clear();
}
