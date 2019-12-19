#include "robot_control/robot_joint_client.h"

namespace{

}	// end of anonymous namespace



namespace robot_control
{

/*****************************public********************************/

RobotJointClient::RobotJointClient(ethercat::EtherCatManager& manager, int slave_no)
{

	memset(&client, 0x00, sizeof(elmo_control::ElmoClient));
    client = new elmo_control::ElmoClient( manager, slave_no);

    printf("Initialize EtherCATJoint (reset)\n");
    client->reset();

    printf("Initialize EtherCATJoint (InterpolationTimePeriod)\n");
    client->setInterpolationTimePeriod(DEFAULT_INTERPOLATION_TIME_PERIOD / 1000);   /* (/1000): ms */

    // servo on
    printf("Initialize EtherCATJoint (servoOn)\n");
    client->servoOn();

    printf("Initialize EtherCATJoint (readInputs)\n");
	memset(&input, 0x00, sizeof(elmo_control::ElmoInput));
    input = client->readInputs();
    output.target_position  = input.position_actual_value;

    printf("Set motor rate torque %.2f mN.m\n",RATE_TORQUE);
    client->setMotorRateTorque(RATE_TORQUE);



	printf("Set motor Acc & Dcc...\n");
	client->setAcc(DATA_TO_COUNT(ACC));
  	client->setDcc(DATA_TO_COUNT(DCC));
	  
	printf("Set motor max torque %d mN.m\n",MAX_TORQUE);
    memset(&output, 0x00, sizeof(elmo_control::ElmoOutput));
    output.max_torque    = int16_t(MAX_TORQUE * 1000.0 / RATE_TORQUE);    
	output.controlword   = 0x001f; // move to operation enabled + new-set-point (bit4) + change set immediately (bit5)
	client->writeOutputs(output);
    
    while ( ! (input.statusword & 0x1000) ) {// bit12 (set-point-acknowledge)
		client->writeOutputs(output);
		input = client->readInputs();
		rt_timer_spin(DEFAULT_INTERPOLATION_TIME_PERIOD);  
    }
	
    printf("Initialize EtherCATJoint (clear new set point)\n");
    output.controlword  &= ~0x0010; // clear new-set-point (bit4)
    client->writeOutputs(output);

    printf("Initialize EtherCATJoint .. done\n");


}

RobotJointClient::~RobotJointClient(){

	// rt_task_delete(&task_);
	shutdown();
	delete(client);
}

void RobotJointClient::motor_halt(){

	output.controlword |= (1 << 8);
	client->writeOutputs(output);
}

void RobotJointClient::motor_halt_continue(){

	output.controlword &= ~(1 << 8);
	client->writeOutputs(output);
}

void RobotJointClient::motor_quick_stop(){
	output.controlword |= (1 << 2);
	client->writeOutputs(output);

}

void RobotJointClient::motor_quick_stop_continue(){
	client->reset();
	client->servoOn();
}  

void RobotJointClient::sentPos(float pos){
    // output.controlword |= 0x0f;
	output.target_position = DATA_TO_COUNT(pos);
	client->writeOutputs(output);
}

void RobotJointClient::sentPosOffset(float pos){
	output.position_offset = DATA_TO_COUNT(pos);
	client->writeOutputs(output);
}


void RobotJointClient::sentVel(float vel){
	if((input.operation_mode == CYCLIC_SYNCHRONOUS_VELOCITY_MODE) || (input.operation_mode == PROFILE_VELOCITY_MODE)){
		output.target_velocity = DATA_TO_COUNT(vel);
		client->writeOutputs(output);
	}else if(input.operation_mode == PROFILE_POSITION_MODE){
		client->setProfileVelocity(DATA_TO_COUNT(vel));
	}
}

void RobotJointClient::sentVelOffset(float vel){
	output.velocity_offset = DATA_TO_COUNT(vel);
	client->writeOutputs(output);
}


void RobotJointClient::sentTorque(float torque){
    output.controlword |= 0x0f;
	output.target_torque = TORQUE_USER_TO_MOTOR(torque);

	client->writeOutputs(output);
}

void RobotJointClient::sentTorqueOffset(float torque){

	output.torque_offset = TORQUE_USER_TO_MOTOR(torque);
	client->writeOutputs(output);
}

int RobotJointClient::sentPosTraj(std::vector<float>points){

	if(input.operation_mode == CYCLIC_SYNCHRONOUS_POSITION_MODE){

		int8_t buffsize_pos;
		std::vector<float>::iterator point = points.begin();
		
		while (1)
		{
			buffsize_pos = client->readActualBuffPos();

			if(buffsize_pos <= ACTUAL_INTERPOLATION_BUFF_SIZE_LIMIT){

				for(int i=0; i<= MAX_INTERPOLATION_BUFF_SIZE / 3; i++){
					if(point != points.end()){
						output.target_position = DATA_TO_COUNT(*point);
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

	RTIME now, previous;	/*ns */
	previous = rt_timer_read();
	int timeout = 10; 	/* 10s */

	while(input.operation_mode != opmode ){
		output.operation_mode = opmode;
		client->writeOutputs(output);
		rt_timer_spin(DEFAULT_INTERPOLATION_TIME_PERIOD);  
		input = client->readInputs();

		now = rt_timer_read();

		if((now - previous) / 1e9 >= timeout ){	
			break;
		} 
	}

	if(input.operation_mode == opmode)
	{
		printf("Operational Mode is : ");
		client->printPDSOperation(input);
		return 0;
	}else{
		fprintf(stderr,"Set operation mode error...");
		return -1;
	}
}

PDS_OPERATION RobotJointClient::readOpmode(){
	return(PDS_OPERATION(input.operation_mode));
}


float RobotJointClient::getMotorPos() const{
	return float(COUNT_TO_DATA(input.position_actual_value));
}

float RobotJointClient::getMotorVel() const{
	return float(COUNT_TO_DATA(input.velocity_actual_value));
}

    uint16 RobotJointClient::getStatusWord() const{
		return(input.statusword);
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

void RobotJointClient::get_feedback(void){
	input = client->readInputs();
}


void RobotJointClient::shutdown(){

	client->printPDSStatus(input);
	client->printPDSOperation(input);
	client->reset();
	client->servoOff();
}


int8_t RobotJointClient::getBuffSize(){
	return ( client->readTrajBuffSize());
}

int8_t RobotJointClient::getBuffPos(){
	return ( client->readActualBuffPos());
}

int16_t RobotJointClient::getChipTemp(){
	return (client->readChipTemp());
} 


/********************************************************************/
}// end of robot_control namespace
