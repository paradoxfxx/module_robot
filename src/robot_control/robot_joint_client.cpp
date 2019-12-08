#include "robot_control/robot_joint_client.h"

namespace robot_control
{

/*****************************public********************************/

RobotJointClient::RobotJointClient(ethercat::EtherCatManager& manager, int slave_no){

    client = new elmo_control::ElmoClient( manager, slave_no);

    printf("Initialize EtherCATJoint (reset)\n");
    client->reset();

    printf("Initialize EtherCATJoint (InterpolationTimePeriod)\n");
    client->setInterpolationTimePeriod(DEFAULT_INTERPOLATION_TIME_PERIOD);     

    printf("Initialize EtherCATJoint (InterpolationBuffSetting)\n");
	client->setInterpolationBuffSize();

    // servo on
    printf("Initialize EtherCATJoint (servoOn)\n");
    client->servoOn();

     // get current positoin
    printf("Initialize EtherCATJoint (readInputs)\n");
    input = client->readInputs();
    // int32 current_position = input.position_actual_value;

    printf("set motor rate torque %d mN.m\n",RATE_TORQUE);
    client->setMotorRateTorque(RATE_TORQUE);

    memset(&output, 0x00, sizeof(elmo_control::ElmoOutput));
    output.controlword   = 0x001f; // move to operation enabled + new-set-point (bit4) + change set immediately (bit5)

    while ( ! (input.statusword & 0x1000) ) {// bit12 (set-point-acknowledge)
		client->writeOutputs(output);
		input = client->readInputs();
		usleep(1000);
    }
    printf("Initialize EtherCATJoint (clear new set point)\n");
    output.controlword  &= ~0x0010; // clear new-set-point (bit4)

    printf("set default op mode csp.\n");
    output.operation_mode = CYCLIC_SYNCHRONOUS_POSITION_MODE; // (csp) cyclic synchronous position mode

    client->writeOutputs(output);

    // waiting until operation_mode is csp
    while(1){
		input = client->readInputs();
		if (client->getPDSOperation(input) != CYCLIC_SYNCHRONOUS_POSITION_MODE)
		{
			output.operation_mode = CYCLIC_SYNCHRONOUS_POSITION_MODE;
			client->writeOutputs(output);
		}else 
			break;
		usleep(1000);
    }  

    printf("Initialize EtherCATJoint .. done\n");

}

RobotJointClient::~RobotJointClient(){
	shutdown();
	delete(client);
}
    
void RobotJointClient::sentpos(float pos){

	output.target_position = DATA_TO_COUNT(pos);
	client->writeOutputs(output);
}

void RobotJointClient::sentvel(float vel){

	output.target_velocity = DATA_TO_COUNT(vel);
	client->writeOutputs(output);
}

void RobotJointClient::sentorque(float torque){

	output.target_torque = TORQUE_USER_TO_MOTOR(torque);
	client->writeOutputs(output);
}

int RobotJointClient::sentPosTraj(std::vector<float>points){

	if(input.operation_mode == CYCLIC_SYNCHRONOUS_POSITION_MODE){

		uint8_t elmo_buffsize;
		std::vector<float>::iterator point = points.begin();
		
		while (1)
		{
			elmo_buffsize = client->readTrajBuffSize();

			if(elmo_buffsize <= ACTUAL_INTERPOLATION_BUFF_SIZE_LIMIT){

				for(int i=0; i<= MAX_INTERPOLATION_BUFF_SIZE / 3; i++){
					if(point != points.end()){
						output.target_position = *point;
						client->writeOutputs(output);
						++ point;
					}else{
						break;
					}	
				}
			}
			rt_timer_spin(DEFAULT_INTERPOLATION_TIME_PERIOD);  
		}
		printf("Successful execution trajectory...\n");
		return 0;
	}else{
		fprintf(stderr,"Operation mode must be CYCLIC_SYNCHRONOUS_POSITION_MODE \n");
		return -1;
	}
}


bool RobotJointClient::changeOPmode(uint8 opmode){
  
	printf("Changing opmode to %d !\n", opmode );
	client->readInputs();
	while(input.operation_mode != opmode){

		output.operation_mode = opmode;
		client->writeOutputs(output);
		client->readInputs();
		printf("%d,%d\n",input.operation_mode,opmode);
		usleep(1000);
	}
	printf(" Opmode is %d !\n", opmode );
}

float RobotJointClient::getMotorPos() const{
	return (COUNT_TO_DATA(input.position_actual_value));
}

float RobotJointClient::getMotorVel() const{
	return (COUNT_TO_DATA(input.velocity_actual_value));
}

float RobotJointClient::getMotorTorque() const{
	return (TORQUE_MOTOR_TO_USER(input.torque_actual_value));
}

float RobotJointClient::getJointPos() const{

	return (client->getAbsolutePos() * 360.0 / ABSOLUTE_RESOLUTION);
}

float RobotJointClient::getJointVel() const{
	
	return (client->getAbsoluteVel() * 360.0 / ABSOLUTE_RESOLUTION);
}

float RobotJointClient::getJointTorque() const{
	return TORQUE_ALALOG_TRANSITION(input.analog_input);
}

void RobotJointClient::shutdown(){

	client->printPDSStatus(input);
	client->printPDSOperation(input);
	client->reset();
	client->servoOff();
}
/********************************************************************/

}// end of robot_control namespace
