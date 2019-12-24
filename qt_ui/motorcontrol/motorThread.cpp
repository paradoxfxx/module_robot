#include "motorThread.h"
#include<QMessageBox>

bool MotorThread::is_stop_ ;
bool MotorThread::is_halt_;
bool MotorThread::is_quickstop_;

float MotorThread::motor_pos_;
float MotorThread::motor_vel_;
float MotorThread::motor_torque_; 

float MotorThread::joint_pos_;
float MotorThread::joint_vel_;
float MotorThread::joint_torque_; 
float MotorThread::chip_temp_;  

int8_t MotorThread::opmode_; 
                           
std::vector<float> MotorThread::vector_;

bool MotorThread::have_new_command_; 
float MotorThread::pos_command_;
float MotorThread::vel_command_;
float MotorThread::torque_command_;

robot_control::RobotJointClient* MotorThread::motor_1;
ethercat::EtherCatManager * MotorThread::manager_;

MotorThread *MotorThread::sent_feedback_ = new MotorThread();

RT_MUTEX MotorThread::mutex_;
const RTIME MotorThread::mutex_timeout = 1000000; /*1 ms */

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

    rt_task_create(&task_command_,"task_command_",0,THREAD_MOTOR_FEEDBACK_PRIORITY,0);
    rt_task_create(&task_feedback_,"task_feedback_",0,THREAD_MOTOR_COMMAND_PRIORITY,0);
    rt_mutex_create(&mutex_,"xenomai_thread_command_and_feedback");

    rt_task_start(&task_command_, &taskSentCommand, NULL);
    rt_task_start(&task_feedback_, &taskFeedback, NULL);
    
    rt_task_join(&task_command_);
    rt_task_join(&task_feedback_);

    rt_task_delete(&task_command_);
    rt_task_delete(&task_feedback_);
    rt_mutex_delete(&mutex_);

    delete manager_;
    delete motor_1;
}


MotorThread::~MotorThread(){
    // printf("delete thread...\n");    
}

void MotorThread::taskSentCommand(void *){

    rt_task_set_periodic(NULL, TM_NOW, 1000000);    /*1ms */

    bool if_new_halt_command_ = false;

    int8_t if_new_opmode_command_ = 0x0;

    while(1){

		rt_task_wait_period(NULL);

        rt_mutex_acquire(&mutex_,mutex_timeout);

            if(if_new_opmode_command_ != opmode_){
                motor_1->changeOPmode(opmode_);
                if_new_opmode_command_ = opmode_;
            }

            if(have_new_command_){

                switch(opmode_){
                    case PROFILE_POSITION_MODE:
                        motor_1->sentPos(pos_command_);
                        break;
                    case PROFILE_VELOCITY_MODE:
                        motor_1->sentVel(vel_command_);
                        break;
                    case TORQUE_PROFILE_MODE:
                        motor_1->sentTorque(torque_command_);
                        break;
                }
                have_new_command_ = false;
            }

            if(is_quickstop_){
                motor_1->motor_quick_stop();
                motor_1->shutdown();
                rt_mutex_release(&mutex_);
                break;
            }

            if(is_stop_){
                motor_1->shutdown();
                rt_mutex_release(&mutex_);
                break;
            }

            if(if_new_halt_command_ != is_halt_){

                if(is_halt_){
                    motor_1->motor_halt();
                }else{
                    motor_1->motor_halt_continue();
                }
                if_new_halt_command_ = is_halt_;
            }
        rt_mutex_release(&mutex_);

    }
}

void MotorThread::taskFeedback(void *){
    rt_task_set_periodic(NULL, TM_NOW, 1000000);    /*1ms */
    RTIME start , end;
    start = rt_timer_read();

    while((!is_stop_) || (!is_quickstop_)){

    	rt_task_wait_period(NULL);

        rt_mutex_acquire(&mutex_,mutex_timeout);
            motor_1->get_feedback();
            motor_pos_ = motor_1->getMotorPos();
            motor_vel_ = motor_1->getMotorVel();
            motor_torque_ = motor_1->getMotorTorque();

            joint_pos_ = motor_1->getJointPos();
            joint_vel_ = motor_1->getJointVel();
            joint_torque_ = motor_1->getJointTorque();
            chip_temp_ = motor_1->getChipTemp();
        rt_mutex_release(&mutex_);

        vector_.clear();
        vector_.push_back(motor_pos_);
        vector_.push_back(motor_vel_);
        vector_.push_back(motor_torque_);
        vector_.push_back(joint_pos_);
        vector_.push_back(joint_vel_);
        vector_.push_back(joint_torque_);
        vector_.push_back(chip_temp_);
        end = rt_timer_read();
        vector_.push_back((end-start)/1000000.0);
        emit sent_feedback_->uploadFeedback(vector_);
    }   
}

void MotorThread::motorStart(){

    try{
        manager_ = new ethercat::EtherCatManager(NETWORK_CARD_NAME);
        motor_1 = new robot_control::RobotJointClient(*manager_, true);

    }catch(std::exception &e){
        emit sentMotorOpenError(true);
        QMessageBox::warning(NULL, "warning", e.what());

        // /*test code */
        // std::vector<float> test = {1.1,1.1,1,1,1,1,1,1}; 
        // emit sent_feedback_->uploadFeedback(test);
        /*test end */   

        return;
    }
    emit sentMotorOpenError(false);
    is_stop_ = false;
    
    this->start();

 }

void MotorThread::motorStop(){

    is_stop_ = true;

}

void MotorThread::motorHalt(){
  
    is_halt_ = true;

}

void MotorThread::motorHaltContinue(){

    is_halt_ = false;

}

void MotorThread::motorQuickStop(){

    is_quickstop_ = true;

}

void MotorThread::motorQuickStopContinue(){

    is_quickstop_ = false;

}

void MotorThread::changeOpmode(uint8_t mode){
    opmode_ = mode;

}

void MotorThread::sentMotorPos(float position){
    have_new_command_ = true;
    pos_command_ = position;

}

void MotorThread::sentMotorVel(float velocity){
    have_new_command_ = true;
    vel_command_ = velocity;

}

void MotorThread::sentMotorTorque(float torque){
    have_new_command_ = true;
    torque_command_ = torque;

}



