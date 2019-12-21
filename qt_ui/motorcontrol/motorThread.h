
#ifndef MOROETHREAD_H
#define MOROETHREAD_H

#include <QThread>
#include <QMutex>

#include "robot_control/robot_joint_client.h"

class MotorThread : public QThread
{
    Q_OBJECT
public:
    explicit MotorThread();
    ~MotorThread();

protected:
    //线程处理函数
    //不能直接调用，通过start()间接调用
    void run();

signals:

    void uploadFeedback(std::vector<float> list);
    void sentMotorOpenError(bool ifError);

private slots:

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


private:
    robot_control::RobotJointClient* motor_1;
    ethercat::EtherCatManager *manager_;

    float motor_pos_,motor_vel_,motor_torque_; /* feedback motor data */
    float joint_pos_,joint_vel_,joint_torque_; /* feedback joint data */
    float chip_temp_;                          /* feedback chip temperature */
    bool is_stop_, is_halt_, is_quickstop_;    /* flag */
    int8_t opmode_;                            /* cia402 operational mode */
    std::vector<float> vector_;
    bool have_new_command_; 
    float pos_command_, vel_command_, torque_command_;

    QMutex mutex_;

};

#endif // MOROETHREAD_H





