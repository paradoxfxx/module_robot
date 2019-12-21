#include "motorThread.h"
#include<QMessageBox>

MotorThread::MotorThread(){

    /*default value */
    is_stop_ = false;
    is_halt_ = false;
    is_quickstop_ = false;
    opmode_ = CYCLIC_SYNCHRONOUS_POSITION_MODE;
    motor_pos_ = 0;
    motor_vel_ = 0;
    motor_torque_ = 0;
    joint_pos_ = 0;
    joint_vel_ = 0;
    joint_torque_ = 0;
    chip_temp_ = 0;
    pos_command_ = 0;
    vel_command_ = 0;
    torque_command_ = 0;
    have_new_command_ = false;

    vector_.clear();
}

void MotorThread::run(){

    while(! is_stop_){

        mutex_.lock();
            if(have_new_command_){
                if(opmode_ == PROFILE_POSITION_MODE){
                    motor_1->sentPos(pos_command_);
                }else if(opmode_ == PROFILE_VELOCITY_MODE){
                    motor_1->sentVel(vel_command_);
                }else if(opmode_ == TORQUE_PROFILE_MODE){
                    motor_1->sentTorque(torque_command_);
                }
                have_new_command_ = false;
            }

            motor_1->get_feedback();
            motor_pos_ = motor_1->getMotorPos();
            motor_vel_ = motor_1->getMotorVel();
            motor_torque_ = motor_1->getMotorTorque();

            joint_pos_ = motor_1->getJointPos();
            joint_vel_ = motor_1->getJointVel();
            joint_torque_ = motor_1->getJointTorque();
            chip_temp_ = motor_1->getChipTemp();
        mutex_.unlock();

        vector_.clear();
        vector_.push_back(motor_pos_);
        vector_.push_back(motor_vel_);
        vector_.push_back(motor_torque_);
        vector_.push_back(joint_pos_);
        vector_.push_back(joint_vel_);
        vector_.push_back(joint_torque_);
        vector_.push_back(chip_temp_);
        emit uploadFeedback(vector_);

        this->usleep(800); /*800 us */
 
    }
}

MotorThread::~MotorThread(){
    
}

void MotorThread::motorStart(){

    try{
        manager_ = new ethercat::EtherCatManager(NETWORK_CARD_NAME , 1);
        motor_1 = new robot_control::RobotJointClient(*manager_, true);
    }catch(std::exception &e){
        emit sentMotorOpenError(true);
        QMessageBox::warning(NULL, "warning", e.what());                         
        return;
    }
    emit sentMotorOpenError(false);
    is_stop_ = false;
    this->start();

 }

void MotorThread::motorStop(){

    mutex_.lock();
    is_stop_ = true;
    motor_1->shutdown();
    mutex_.unlock();
    delete manager_;
    delete motor_1;
    this->quit();

}

void MotorThread::motorHalt(){
    mutex_.lock();
    is_halt_ = true;
    motor_1->motor_halt();
    mutex_.unlock();
    this->wait();   

}

void MotorThread::motorHaltContinue(){
    this->start();
    mutex_.lock();
    is_halt_ = false;
    motor_1->motor_halt_continue();
    mutex_.unlock();

}

void MotorThread::motorQuickStop(){

    mutex_.lock();
    is_quickstop_ = true;
    motor_1->motor_quick_stop();
    motor_1->shutdown();
    mutex_.unlock();
    delete manager_;
    delete motor_1;
    this->quit();

}

void MotorThread::motorQuickStopContinue(){

    mutex_.lock();
    is_quickstop_ = false;
    motor_1->motor_quick_stop_continue();
    mutex_.unlock();

}

void MotorThread::changeOpmode(uint8_t mode){
    mutex_.lock();
    motor_1->changeOPmode(mode);
    mutex_.unlock();

}

void MotorThread::sentMotorPos(float position){
    mutex_.lock();
    have_new_command_ = true;
    pos_command_ = position;
    mutex_.unlock();

}

void MotorThread::sentMotorVel(float velocity){
    mutex_.lock();
    have_new_command_ = true;
    vel_command_ = velocity;
    mutex_.unlock();

}

void MotorThread::sentMotorTorque(float torque){
    mutex_.lock();
    have_new_command_ = true;
    torque_command_ = torque;
    mutex_.unlock();

}