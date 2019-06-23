#include "robot_control/robot_client.h"

namespace robot_control
{


/*****************************public********************************/

RobotClient::RobotClient(ethercat::EtherCatManager& manager, int slave_no){

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

RobotClient::~RobotClient(){

  shutdown();
  delete(client);
}
    
void RobotClient::sentpos(double pos){

  output.target_position = data_to_count(pos);
  client->writeOutputs(output);
}

void RobotClient::sentvel(double vel){

  output.target_velocity = data_to_count(vel);
  client->writeOutputs(output);
}

void RobotClient::sentorque(double torque){

  output.target_torque = torque_to_motorT(torque);
  client->writeOutputs(output);
}

bool RobotClient::changeOPmode(int opmode){
  
  while(input.operation_mode != opmode){
    printf("Changing opmode to %d !\n", opmode );
    output.operation_mode = opmode;
    client->writeOutputs(output);
    usleep(1e+5);
    client->readInputs();
  }
    printf(" Opmode is %d !\n", opmode );
}


 void RobotClient::getfeedback(){
  client->readInputs();

  pos_f = count_to_data(input.position_actual_value);
  vel_f = count_to_data(input.velocity_actual_value);
  torque_f = motorT_to_torque(input.torque_actual_value);

}


double RobotClient::getpos(){
  return pos_f;
}

double RobotClient::getvel(){
  return vel_f;
}

double RobotClient::getorque(){
  return torque_f;
}

void RobotClient::shutdown(){

  client->printPDSStatus(input);
  client->printPDSOperation(input);
  client->reset();
  client->servoOff();
}
/********************************************************************/




/*****************************private********************************/

int32_t RobotClient::data_to_count(double data){

    return int32_t(data * RESOLUTION * REDUCTION_RATIO / 360);
}

double RobotClient::count_to_data(int32_t count){

    return double(count * 360 / RESOLUTION / REDUCTION_RATIO);
}

int RobotClient::torque_to_motorT(double torque){

    return int(torque * 1000 / RATE_TORQUE);
}

double RobotClient::motorT_to_torque(int torque){

  return double(torque * RATE_TORQUE / 1000);
}
/********************************************************************/


}// end of robot_control namespace
