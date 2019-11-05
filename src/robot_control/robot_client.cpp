#include "robot_control/robot_joint_client.h"

namespace robot_control
{

/*****************************public********************************/

RobotJointClient::RobotJointClient(ethercat::EtherCatManager& manager, int slave_no){

    client = new elmo_control::ElmoClient( manager, slave_no);

    printf("Initialize EtherCATJoint (reset)\n");
    client->reset();

    printf("Initialize EtherCATJoint (InterpolationTimePeriod)\n");
    client->setInterpolationTimePeriod(1);     // 1 msec
    
    // servo on
    printf("Initialize EtherCATJoint (servoOn)\n");
    client->servoOn();

     // get current positoin
    printf("Initialize EtherCATJoint (readInputs)\n");
    input = client->readInputs();
    int32 current_position = input.position_actual_value;

    printf("set motor rate torque %d mN.m\n",RATE_TORQUE);
    client->setMotorRateTorque(RATE_TORQUE);

    // set target position
    memset(&output, 0x00, sizeof(elmo_control::ElmoOutput));

    output.target_torque = 0;    // 0% (unit 0.1%)
    output.max_torque    = MAX_TORQUE;    // 50% (unit 0.1%)
    output.controlword   = 0x001f; // move to operation enabled + new-set-point (bit4) + change set immediately (bit5)

    // set profile velocity
    printf("Initialize EtherCATJoint (setProfileVelocity)\n");
    client->setProfileVelocity(0x2000);

    client->writeOutputs(output);
    while ( ! (input.statusword & 0x1000) ) {// bit12 (set-point-acknowledge)
      input = client->readInputs();
    }
    printf("Initialize EtherCATJoint (clear new set point)\n");
    output.controlword   &= ~0x0010; // clear new-set-point (bit4)


    printf("set default op mode csp.\n");
    output.operation_mode = 0x08; // (csp) cyclic synchronous position mode
    client->writeOutputs(output);

    // waiting until operation_mode is csp
    while(1){
      if(input.operation_mode != 0x08){
        input = client->readInputs();
        output.operation_mode = 0x08;
        client->writeOutputs(output);
      }else break;
    }  

    printf("Initialize EtherCATJoint .. done");

}

RobotJointClient::~RobotJointClient(){

  shutdown();
  delete(client);
}
    
void RobotJointClient::sentpos(double pos){

  output.target_position = DATA_TO_COUNT(pos);
  client->writeOutputs(output);
}

void RobotJointClient::sentvel(double vel){

  output.target_velocity = DATA_TO_COUNT(vel);
  client->writeOutputs(output);
}

void RobotJointClient::sentorque(double torque){

  output.target_torque = TORQUE_USER_TO_MOTOR(torque);
  client->writeOutputs(output);
}

bool RobotJointClient::changeOPmode(int opmode){
  
  printf("Changing opmode to %d !\n", opmode );
  while(input.operation_mode != opmode){
    output.operation_mode = opmode;
    client->writeOutputs(output);
    client->readInputs();
  }
    printf(" Opmode is %d !\n", opmode );
}


 void RobotJointClient::getfeedback(){
  client->readInputs();

  pos_f = COUNT_TO_DATA(input.position_actual_value);
  vel_f = COUNT_TO_DATA(input.velocity_actual_value);
  torque_f = TORQUE_MOTOR_TO_USER(input.torque_actual_value);

}


double RobotJointClient::getpos() const{
  return pos_f;
}

double RobotJointClient::getvel() const{
  return vel_f;
}

double RobotJointClient::getorque() const{
  return torque_f;
}

void RobotJointClient::shutdown(){

  client->printPDSStatus(input);
  client->printPDSOperation(input);
  client->reset();
  client->servoOff();
}
/********************************************************************/

}// end of robot_control namespace
