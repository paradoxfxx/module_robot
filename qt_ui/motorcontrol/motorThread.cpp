#include "motorThread.h"
#include<QMessageBox>

bool MotorThread::is_stop_ ;
bool MotorThread::is_halt_;
bool MotorThread::is_quickstop_;

float MotorThread::motor_pos_;
float MotorThread::motor_vel_;
float MotorThread::motor_torque_; 
float MotorThread::motor_current_;

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

QMutex MotorThread::mutex__;  

MotorThread::MotorThread(){}

void MotorThread::run(){

    /*default value */
    is_stop_        = false;
    is_halt_        = false;
    is_quickstop_   = false;
    opmode_         = PROFILE_POSITION_MODE;
    motor_pos_      = 0;
    motor_vel_      = 0;
    motor_torque_   = 0;
    motor_current_  = 0;
    joint_pos_      = 0;
    joint_vel_      = 0;
    joint_torque_   = 0;
    chip_temp_      = 0;
    pos_command_    = 0;
    vel_command_    = 0;
    torque_command_ = 0;
    have_new_command_ = false;
    vector_.clear();

    int8_t if_new_opmode_command_   = 0x1;
    bool if_new_halt_command_       = false;
	RTIME start,end;
    int temp = 0;

    while(1){

    /*####################### motor command #########################################*/
        start = rt_timer_read();

        mutex__.lock();
        if(if_new_opmode_command_ != opmode_){
            // printf("change opmode\n");
            motor_1->changeOPmode(opmode_);
            if_new_opmode_command_ = opmode_;
        }
        mutex__.unlock();

        mutex__.lock();
        if(have_new_command_){
            switch(opmode_){

                case PROFILE_POSITION_MODE:
                    motor_1->sentPos(pos_command_,vel_command_);
                    // printf("pos command\n");
                    break;
                case PROFILE_VELOCITY_MODE:
                    motor_1->sentVel(vel_command_);
                    // printf("vel command\n");
                    break;
                case TORQUE_PROFILE_MODE:
                    motor_1->sentTorque(torque_command_);
                    // printf("tor command\n");
                    break;
            }
            have_new_command_ = false;
        }
        mutex__.unlock();

        mutex__.lock();
        if(is_quickstop_){
            motor_1->motor_quick_stop();
            motor_1->shutdown();
            // printf("quick stop\n");
            break;
        }
        mutex__.unlock();

        mutex__.lock();
        if(is_stop_){
            motor_1->shutdown();
            // printf("stop\n");
            break;
        }
        mutex__.unlock();

        mutex__.lock();
        if(if_new_halt_command_ != is_halt_){

            if(is_halt_){
                motor_1->motor_halt();
                // printf("halt\n");
            }else{
                motor_1->motor_halt_continue();
                // printf("halt continue\n");
            }
            if_new_halt_command_ = is_halt_;
        }
        mutex__.unlock();

    /*########################### motor command  end ################################*/

    /*########################### motor feedbak ####################################*/

        motor_1->get_feedback();
        motor_pos_      = motor_1->getMotorPos();
        motor_vel_      = motor_1->getMotorVel();
        motor_torque_   = motor_1->getMotorTorque();
        motor_current_  = motor_1->getMotorCurrent();
        joint_pos_      = motor_1->getJointPos();
        joint_vel_      = motor_1->getJointVel();
        joint_torque_   = motor_1->getJointTorque();

        if(temp % 300 == 1){    // 5s update  
            chip_temp_  = motor_1->getChipTemp();
            temp = 0;
        }

        vector_.clear();
        vector_.push_back(motor_pos_);
        vector_.push_back(motor_vel_);
        vector_.push_back(motor_torque_);
        vector_.push_back(motor_current_);
        vector_.push_back(joint_pos_);
        vector_.push_back(joint_vel_);
        vector_.push_back(joint_torque_);
        vector_.push_back(chip_temp_);

        rt_timer_spin(1e7); //  10ms
        temp += 1;

        end = rt_timer_read();
        vector_.push_back((end-start)/1000000.0);
        emit sent_feedback_->uploadFeedback(vector_);

    /*########################### motor feedbak end ################################*/
    }
    mutex__.unlock();

    delete manager_;
    delete motor_1;

    // printf("stop motor thread.\n");
}


MotorThread::~MotorThread(){
    // printf("delete thread...\n");    
}

void MotorThread::motorStart(){

    try{
        manager_ = new ethercat::EtherCatManager(NETWORK_CARD_NAME);
        motor_1 = new robot_control::RobotJointClient(*manager_,1);

    }catch(std::exception &e){
        emit sentMotorOpenError(true);
        QMessageBox::warning(NULL, "warning", e.what());
        return;
    }
    emit sentMotorOpenError(false);
    mutex__.lock();
    is_stop_ = false;
    mutex__.unlock();

    this->start();  

 }

void MotorThread::motorStop(){
    // printf("is_stop:%s\n",is_stop_?"true":"false");
    mutex__.lock();
    is_stop_ = true;
    mutex__.unlock();
}

void MotorThread::motorHalt(){
    mutex__.lock();
    is_halt_ = true;
    mutex__.unlock();

}

void MotorThread::motorHaltContinue(){
    mutex__.lock();
    is_halt_ = false;
    mutex__.unlock();

}

void MotorThread::motorQuickStop(){
    mutex__.lock();
    is_quickstop_ = true;
    mutex__.unlock();

}

void MotorThread::motorQuickStopContinue(){
    mutex__.lock();
    is_quickstop_ = false;
    mutex__.unlock();

}

void MotorThread::changeOpmode(uint8_t mode){
    mutex__.lock();
    opmode_ = mode;
    mutex__.unlock();

}

void MotorThread::sentMotorPos(std::vector<float> data){
    // printf("pos command\n");
    mutex__.lock();
    have_new_command_ = true;
    pos_command_ = data[0] ;
    vel_command_ = data[1] ;
    mutex__.unlock();

}

void MotorThread::sentMotorVel(float velocity){
    // printf("vel command\n");
    mutex__.lock();
    have_new_command_ = true;
    vel_command_ = velocity ;
    mutex__.unlock();

}

void MotorThread::sentMotorTorque(float torque){
    // printf("tor command\n");
    mutex__.lock();
    have_new_command_ = true;
    torque_command_ = torque;
    mutex__.unlock();
}