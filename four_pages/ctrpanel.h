#ifndef CTRPANEL_H
#define CTRPANEL_H

#include <QWidget>

namespace Ui {
class Ctrpanel;
}

class Ctrpanel : public QWidget
{
    Q_OBJECT

public:
    Ui::Ctrpanel *ui;
    explicit Ctrpanel(QWidget *parent = 0);
    void Blog();

    ~Ctrpanel();

private slots:

    void receive_frame(QString);           //获取到由主页面发来的整个帧数据，准备解用。

    void save_operationblog(QString);

    void on_mcu_ox00_checkbox_clicked(bool checked);

    void on_mcu_ox01_checkBox_clicked(bool checked);

    void on_mcu_ox02_checkbox_clicked(bool checked);

    void on_sendButton_clicked();

//    void receive_filepath(QString, bool);


signals:
    void send_option(QString,bool,int);    //选择后复选框后将选择的数据发送给mainwindow再给shoupanel页面
//    void cancel_option(QString);
    void send_operation_messgae(int, QString, QColor);

private:
    QByteArray Frame;
    void initial();
    void operation(QString,bool);
};

#endif // CTRPANEL_H
