#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <QWidget>

namespace Ui {
class Calibration;
}

class Calibration : public QWidget
{
    Q_OBJECT

public:
    explicit Calibration(QWidget *parent = 0);
    ~Calibration();

signals:
    void send_frame(QString);

private slots:

    void on_cal_0x00_lineEdit_editingFinished();

    void on_cal_0x01_lineEdit_editingFinished();

    void on_cal_0x02_lineEdit_editingFinished();

    void on_cal_0x03_lineEdit_editingFinished();

    void on_cal_0x04_lineEdit_editingFinished();

    void on_cal_0x05_lineEdit_editingFinished();

    void on_cal_0x06_lineEdit_editingFinished();

    void on_cal_0x07_lineEdit_editingFinished();

    void on_cal_0x08_lineEdit_editingFinished();

    void on_cal_0x09_lineEdit_editingFinished();

    void on_cal_0x0A_lineEdit_editingFinished();

    void on_cal_0x0B_lineEdit_editingFinished();

    void on_cal_0x0C_lineEdit_editingFinished();

    void on_cal_0x0D_lineEdit_editingFinished();

    void on_cal_0x0E_lineEdit_editingFinished();

    void on_cal_0x0F_lineEdit_editingFinished();

    void on_cal_0x10_lineEdit_editingFinished();

    void on_cal_0x11_lineEdit_editingFinished();

    void on_cal_0x12_lineEdit_editingFinished();

    void on_cal_0x13_lineEdit_editingFinished();

    void on_cal_0x14_lineEdit_editingFinished();

    void on_cal_0x15_lineEdit_editingFinished();


private:
    Ui::Calibration *ui;
    void sendEdit(int, QString);
};

#endif // CALIBRATION_H
