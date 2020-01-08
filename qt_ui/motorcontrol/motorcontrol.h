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

    void sentMotorPos(std::vector<float>);
    void sentMotorVel(float velocity);
    void sentMotorTorque(float torque);

    void sentPlotPosData(std::vector<float>);
    void sentPlotVelData(std::vector<float>);
    void sentPlotTorData(std::vector<float>);
    void sentPlotTempData(std::vector<float>);


private slots:
    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_2_clicked(bool checked);

    void on_pushButton_3_clicked();

    void on_tabWidget_tabBarClicked(int index);

    void on_pushButton_5_clicked();

    void on_actionQuit_triggered();

    void on_actionHelp_triggered();

    void motor_start_error(bool ifError);

    void get_feedback_data(std::vector<float> vector);

    void on_actionPostion_triggered();

    void on_actionVeclocity_triggered();

    void on_actionTorque_triggered();

    void on_actionTemperature_triggered();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

private:

    Ui::MotorControl *ui;
    StatePlot *ui_state_plot_pos;
    StatePlot *ui_state_plot_vel;
    StatePlot *ui_state_plot_tor;
    StatePlot *ui_state_plot_temp;

    MotorThread *thread_;
    static bool plot_pos;
    static bool plot_vel;
    static bool plot_tor;
    static bool plot_temp;

    static std::vector<float>plot_data;
};

#endif // MOTORCONTROL_H
