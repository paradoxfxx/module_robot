#ifndef ROBOT_CONTROL_H
#define ROBOT_CONTROL_H

#include <elmo_control/elmo_client.h>
#include <ethercat_manager/ethercat_manager.h>
#include "common.h"

namespace robot_control
{

class RobotJointClient
{

public:

    /**
    * \manage : enternet card name(string)
    * \slave_no : slave number
    */
    RobotJointClient(ethercat::EtherCatManager& manager, int slave_no);

    /**
    * \brief pause motor operation
    * \return int (0:success else:error)
    */ 
    void motor_halt();

    /**
    * \brief continue motor operation
    * \return int (0:success else:error)
    */
    void  motor_halt_continue();

    void motor_quick_stop();

    void motor_quick_stop_continue();

    /**
    * \brief sent position to the motor
    * \unit degree
    * \return void
    */
    void sentPos(float pos);
    void sentPosOffset(float pos);

    /**
    * \brief sent velocity to the motor
    * \unit degree/s
    * \return void
    */
    void sentVel(float vel);
    void sentVelOffset(float vel);

    /**
    * \brief sent torque to the motor
    * \unit mN.m
    * \return void
    */
    void sentTorque(float torque);
    void sentTorqueOffset(float torque);

    /**
    * \brief sent trajectory to the motor
    * \unit degree
    * \return int(0:success, -1:error)
    */
    int sentPosTraj(std::vector<float>points);

    /**
    * \brief change the operation mode. 
    * \return bool
    */
    bool changeOPmode(uint8 opmode);

    PDS_OPERATION readOpmode();

    /**
    * \brief update the motor state
    * \unit 
    * \return void 
    */
    void get_feedback(void);

    /**
    * \brief get the motor pos 
    * \unit degree
    * \return float
    */
    float getMotorPos() const;

    /**
    * \brief get the motor vel
    * \unit degree/s
    * \return float
    */
    float getMotorVel() const;

    /**
    * \brief get the motor torque
    * \unit  mN.m
    * \return float
    */
    float getMotorTorque() const;

    
    uint16 getStatusWord() const;
    /**
    * \brief get the joint pos 
    * \unit degree
    * \return float
    */
    float getJointPos() const;

    /**
    * \brief get the joint vel
    * \unit degree/s
    * \return float
    */
    float getJointVel() const;

    /**
    * \brief get the joint torque sensor value.
    * \unit mN.m
    * \return float
    */
    float getJointTorque() const;

    /**
    * \brief shutdown the communication. 
    * \return void
    */
    void shutdown();


    /*test code  */
    int8_t getBuffSize();
    int8_t getBuffPos();
    /*end */

    int16_t getChipTemp(); 

    ~RobotJointClient();

private:
    
    // static void get_feedback(void *stop_flag);



    elmo_control::ElmoClient* client;
    elmo_control::ElmoInput input;
    elmo_control::ElmoOutput output;

	

};


} 
#endif
