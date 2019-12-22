#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

#include <QMainWindow>

#include "stateplot.h"
#include "motorThread.h"

namespace Ui {
class MotorControl;
}

class MotorControl : public QMainWindow
{
    Q_OBJECT

public:
    explicit MotorControl(QWidget *parent = nullptr);
    ~MotorControl();

signals:

    void motorStart();
    void motorStop();
    void motorHalt();
    void motorHaltContinue();
    void motorQuickStop();
    void motorQuickStopContinue();
    
    void changeOpmode(uint8_t mode);

    void sentMotorPos(float position);
    void sentMotorVel(float velocity);
    void sentMotorTorque(float torque);


private slots:
    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_2_clicked(bool checked);

    void on_pushButton_3_clicked();

    void on_tabWidget_tabBarClicked(int index);

    void on_pushButton_5_clicked();

    void on_actionStatePlot_triggered();


    void on_actionQuit_triggered();

    void on_actionHelp_triggered();

    void motor_start_error(bool ifError);

    void get_feedback_data(std::vector<float> vector);

private:

    Ui::MotorControl *ui;
    StatePlot *ui_state_plot;
    MotorThread *thread_;
};

#endif // MOTORCONTROL_H
