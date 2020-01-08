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
    static MotorThread *sent_feedback_;

protected:
    void run();

signals:

    void uploadFeedback(std::vector<float> vector);
    void sentMotorOpenError(bool ifError);

private slots:

    void motorStart();
    void motorStop();
    void motorHalt();
    void motorHaltContinue();
    void motorQuickStop();
    void motorQuickStopContinue();
    
    void changeOpmode(uint8_t mode);

    void sentMotorPos(std::vector<float> data);
    void sentMotorVel(float velocity);
    void sentMotorTorque(float torque);

private:
    static robot_control::RobotJointClient* motor_1;
    static ethercat::EtherCatManager *manager_;

    static float motor_pos_,motor_vel_,motor_torque_,motor_current_;    /* feedback motor data */
    static float joint_pos_,joint_vel_,joint_torque_;                   /* feedback joint data */
    static float chip_temp_;                                            /* feedback chip temperature */
    static bool is_stop_, is_halt_, is_quickstop_;                      /* flag */
    static int8_t opmode_;                                              /* cia402 operational mode */
    static std::vector<float> vector_;                                  /* qt transmit data */
    static bool have_new_command_;                                      
    static float pos_command_, vel_command_, torque_command_;           
    
    static QMutex mutex__;  


};

#endif // MOROETHREAD_H