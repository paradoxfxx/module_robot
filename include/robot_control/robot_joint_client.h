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
    * \brief sent position to the motor
    * \unit degree
    * \return void
    */
    void sentpos(float pos);

    /**
    * \brief sent velocity to the motor
    * \unit degree/s
    * \return void
    */
    void sentvel(float vel);

    /**
    * \brief sent torque to the motor
    * \unit mN.m
    * \return void
    */
    void sentorque(float torque);

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

    ~RobotJointClient();

private:
    
    elmo_control::ElmoClient* client;
    elmo_control::ElmoInput input;
    elmo_control::ElmoOutput output;

    float pos_f;
    float vel_f;
    float torque_f;

};


} 
#endif
