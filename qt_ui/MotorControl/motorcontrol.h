#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

#include <QWidget>

namespace Ui {
class MotorControl;
}

class MotorControl : public QWidget
{
    Q_OBJECT

public:
    explicit MotorControl(QWidget *parent = nullptr);
    ~MotorControl();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

    void on_tabWidget_tabBarClicked(int index);

    void on_pushButton_5_clicked();

    void on_pushButton_2_clicked(bool checked);

private:
    Ui::MotorControl *ui;
};

#endif // MOTORCONTROL_H
