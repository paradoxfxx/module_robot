#ifndef ROBOT_CONTROL_H
#define ROBOT_CONTROL_H

#include <elmo_control/elmo_client.h>
#include <ethercat_manager/ethercat_manager.h>
#include "common.h"

namespace robot_control
{

class RobotClient
{

public:

    /**
    * \manage : enternet card name(string)
    * \slave_no : slave number
    */
    RobotClient(ethercat::EtherCatManager& manager, int slave_no);

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

    double getpos();

    double getvel();

    double getorque();

    /**
    * \brief shutdown the communication. 
    * \return void
    */
    void shutdown();

    ~RobotClient();


private:
    /**
    * \brief convert the data(pos,vel,acc,dcc) to motor pulse.
    * \return int
    */
    static int32_t data_to_count(double data);

    /**
    * \brief convert the motor pulse to data(pos,vel,acc,dcc).
    * \return int
    */
    static double count_to_data(int32_t count);

    /**
    * \brief convert user torque unit(mN.m) to elmo unit(per thousand of rate torque). 
    * \return int
    */
    static int torque_to_motorT(double torque);

    /**
    * \brief convert elmo unit(per thousand of rate torque) to user torque unit(mN.m). 
    * \return double
    */
    static double motorT_to_torque(int torque);



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
