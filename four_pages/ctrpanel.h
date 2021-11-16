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

    void on_mcu_ox03_checkbox_clicked(bool checked);

    void on_mcu_ox04_checkbox_clicked(bool checked);

    void on_mcu_ox05_checkbox_clicked(bool checked);

    void on_mcu_ox06_checkbox_clicked(bool checked);

    void on_mcu_ox07_checkbox_clicked(bool checked);

    void on_mcu_ox08_checkbox_clicked(bool checked);

    void on_mcu_ox09_checkbox_clicked(bool checked);

    void on_mcu_ox0A_checkbox_clicked(bool checked);

    void on_mcu_ox0B_checkbox_clicked(bool checked);

    void on_mcu_ox0C_checkbox_clicked(bool checked);

    void on_mcu_ox0D_checkbox_clicked(bool checked);

    void on_mcu_ox0E_checkbox_clicked(bool checked);

    void on_mcu_ox0F_checkbox_clicked(bool checked);

    void on_mcu_ox10_checkbox_clicked(bool checked);

    void on_mcu_ox11_checkbox_clicked(bool checked);

    void on_mcu_ox12_checkbox_clicked(bool checked);

    void on_mcu_ox13_checkbox_clicked(bool checked);

    void on_mcu_ox14_checkbox_clicked(bool checked);

    void on_mcu_ox15_checkbox_clicked(bool checked);

    void on_mcu_ox16_checkbox_clicked(bool checked);

    void on_mcu_ox17_checkbox_clicked(bool checked);

    void on_mcu_ox18_checkbox_clicked(bool checked);

    void on_mcu_ox19_checkbox_clicked(bool checked);

    void on_mcu_ox1A_checkbox_clicked(bool checked);


signals:
    void send_option(QString,bool,int);    //选择后复选框后将选择的数据发送给mainwindow再给shoupanel页面

    void send_operation_messgae(int, QString, QColor);

    void send_frame(QString);

private:
    QByteArray Frame;
    void operation(QString,bool);
};

#endif // CTRPANEL_H
