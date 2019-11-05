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
    void sentpos(double pos);

    /**
    * \brief sent velocity to the motor
    * \unit degree/s
    * \return void
    */
    void sentvel(double vel);

    /**
    * \brief sent torque to the motor
    * \unit mN.m
    * \return void
    */
    void sentorque(double torque);

    /**
    * \brief change the operation mode. 
    * \return bool
    */
    bool changeOPmode(int opmode);

    /**
    * \brief get the link pos , vel , torque
    * \unit degree ,degree/s , mN.m
    * \return double
    */
    void getfeedback();

    double getpos() const;

    double getvel() const;

    double getorque() const;

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

    double pos_f;
    double vel_f;
    double torque_f;
};


} 
#endif
