#include <robot_controller/pid_controller.h>

#include <iostream>

namespace controller
{

PID_controller::PID_controller()
{
   kp_ = 1;
   ki_ = 1;
   kd_ = 1;
   integrator_ = 0;
   previous_error_ = 0;
   integrator_limit_ = INFINITY;
   frequency_ = 1;
}

void PID_controller::pid_set_gains(float kp, float ki, float kd)
{
   kp_ = kp;
   ki_ = ki;
   kd_ = kd;
}

void PID_controller::pid_get_gains(float &kp, float &ki, float &kd)
{
    kp = kp_;
    ki = ki_;
    kd = kd_;
}

float PID_controller::pid_get_integral_limit()
{
    return integrator_limit_;
}

float PID_controller::pid_get_integral()
{
    return integrator_;
}

float PID_controller::pid_process(float error)
{
    float output;
    integrator_ += error;

    if (integrator_ > integrator_limit_) {

        integrator_ = integrator_limit_;
        
    } else if (integrator_ < - integrator_limit_) {

        integrator_ = - integrator_limit_;
    }

    output  = - kp_ * error;
    output += - ki_ * integrator_ / frequency_;
    output += - kd_ * (error - previous_error_) * frequency_;

    previous_error_ = error;
    return output;
}

void PID_controller::pid_set_integral_limit(float max)
{
    integrator_limit_ = max;
}

void PID_controller::pid_reset_integral()
{
    integrator_ = 0.;
}

void PID_controller::pid_set_frequency(float frequency)
{
    frequency_ = frequency;
}

float PID_controller::pid_get_frequency()
{
    return frequency_;
}

}