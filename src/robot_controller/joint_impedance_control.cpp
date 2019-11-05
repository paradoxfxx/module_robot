#include "robot_controller/joint_impedance_controler.h"
#include <cmath>
#include <iostream>

JointImpedanceControl::JointImpedanceControl(){

    inertia = 1;
    damping = 10;
    stiffness = 1000;

    tar_pos = 0;
    tar_vel = 0;
    tar_acc = 0;

    err_pos = 0;
    err_vel = 0;
    err_acc = 0;

    torque = 0;

    torque_threshold = 50;

    std::cout<<"--------------------\n";
    std::cout<< "Default param:\n";
    std::cout<<"inertia:"<<inertia<<"\n";
    std::cout<<"damping:"<<damping<<"\n";
    std::cout<<"stiffness:"<<stiffness<<"\n";
    std::cout<<"torque_threshold:"<<torque_threshold<<"\n";
    std::cout<<"--------------------\n";

}

void JointImpedanceControl::set_stiffness(double StiffnessScalar){
    stiffness = StiffnessScalar;
}

void JointImpedanceControl::set_damping(double DampingScalar){
    damping = DampingScalar;
}

void JointImpedanceControl::set_inertia(double IneratiaScalar){
    inertia = IneratiaScalar;
}

void JointImpedanceControl::sent_plan(double &pos, double &vel, double &acc){
    tar_pos = pos;
    tar_vel = vel;
    tar_acc = acc;
 }

 void JointImpedanceControl::set_torque_threshold(int threshold){

    torque_threshold = threshold;     
 }

void JointImpedanceControl::update_err(double &jnt_pos, double &jnt_vel, double &jnt_acc){

    torque = 0;

    err_pos = tar_pos - jnt_pos;
    err_vel = tar_vel - jnt_vel;
    err_acc = tar_acc - jnt_acc;

    torque += inertia * err_acc;
    torque += damping * err_vel;
    torque += stiffness * err_pos;

    if (fabs(torque) > torque_threshold)
    {
        if(torque >= 0)
        {
            torque = torque_threshold;
        }else
        {
            torque = - torque_threshold;
        }
    }

}

void JointImpedanceControl::get_output(double &jnt_tor){

    jnt_tor = torque;
}


void JointImpedanceControl::get_err(double &pos_err, double &vel_err, double &acc_err){

    pos_err = err_pos;
    vel_err = err_vel;
    acc_err = err_acc;
}


