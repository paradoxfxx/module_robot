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

    // servo on
    printf("Initialize EtherCATJoint (servoOn)\n");
    client->servoOn();

    printf("Initialize EtherCATJoint (InterpolationTimePeriod)\n");
    client->setInterpolationTimePeriod(DEFAULT_INTERPOLATION_TIME_PERIOD / 1000);   /* (/1000): ms */

	// printf("Set position range limit\n");
	// client->setPosRangeLimit(DATA_TO_COUNT(POS_DOWN_LIMIT),DATA_TO_COUNT(POS_UP_LIMIT));

	printf("Set position option code\n");
	uint16_t temp = client->readPosOptionCode();
	client->setPosOptionCode(temp |= 0xff00);

    printf("Initialize EtherCATJoint (readInputs)\n");
	memset(&input, 0x00, sizeof(elmo_control::ElmoInput));
    input = client->readInputs();
    output.target_position  = input.position_actual_value;

    printf("Set motor rate current %d mA\n",RATE_CURRENT);
	client->setRateCurrent(RATE_CURRENT);

    printf("Set motor max current %d mA\n",MAX_CURRENT);
	client->setMaxCurrent(MAX_CURRENT * 1000.0 / RATE_CURRENT);

    printf("Set motor rate torque %.2f mN.m\n",RATE_TORQUE);
    client->setMotorRateTorque(RATE_TORQUE);

	printf("Set motor Acc & Dcc...\n");
	client->setAcc(DATA_TO_COUNT(ACC));
  	client->setDcc(DATA_TO_COUNT(DCC));
	printf("Set motor MaxAcc & MaxDcc...\n");
	client->setMaxAcc(DATA_TO_COUNT(MAXACC));
  	client->setMaxDcc(DATA_TO_COUNT(MAXDCC));
	printf("Set motor MaxProfileVelocity...\n");
  	client->setMaxProfileVelocity(DATA_TO_COUNT(MAX_PROFILE_VELOCITY));

	printf("Set motor max torque %d mN.m\n",MAX_TORQUE);
    memset(&output, 0x00, sizeof(elmo_control::ElmoOutput));
    output.max_torque    = int16_t(MAX_TORQUE * 1000.0 / RATE_TORQUE);    
	output.controlword   |= 0x001f; // move to operation enabled + new-set-point (bit4) + change set immediately (bit5)
	output.operation_mode = PROFILE_POSITION_MODE;
	client->writeOutputs(output);
    
    while ( !(input.statusword & 0x1000) ) {// bit12 (set-point-acknowledge)
		client->writeOutputs(output);
		client->servoOn();
		input = client->readInputs();
		output.target_position  = input.position_actual_value;
		printf("status word: %02x\n",input.statusword);
		rt_timer_spin(DEFAULT_INTERPOLATION_TIME_PERIOD);  
    }
	
    printf("Initialize EtherCATJoint (clear new set point)\n");
    output.controlword  &= ~0x0010; // clear new-set-point (bit4)
    client->writeOutputs(output);

    printf("Initialize EtherCATJoint .. done\n");

	// uint32_t torque = client->readMotorRateTorque();
	// printf("%d\n",torque);
}

RobotJointClient::~RobotJointClient(){

	shutdown();
	delete client;
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
	output.controlword   |= 0x000f;
	output.target_position = DATA_TO_COUNT(pos);
	client->writeOutputs(output);
}


void RobotJointClient::sentPos(float pos, float vel){

	if(input.operation_mode == PROFILE_POSITION_MODE){
		
		output.controlword &=  ~(1 << 4);
		output.controlword &=  ~(1 << 5);
		output.controlword &=  ~(1 << 6);
		output.controlword &=  ~(1 << 8);
		output.controlword &=  ~(1 << 9);
		client->writeOutputs(output);

		client->setProfileVelocity(DATA_TO_COUNT(vel));
		output.target_position = DATA_TO_COUNT(pos);
		client->writeOutputs(output);


		output.controlword |=  (1 << 5);
		output.controlword |=  (1 << 4);
		client->writeOutputs(output);


	}
} 


void RobotJointClient::sentPosDemandValue(float pos){
	client->sentPosDemandValue(DATA_TO_COUNT(pos));
}	


void RobotJointClient::sentPosOffset(float pos){
	output.controlword |=  0x000f;
	output.position_offset = DATA_TO_COUNT(pos);
	client->writeOutputs(output);
}


void RobotJointClient::sentVel(float vel){
	output.controlword |=  0x000f;
	output.target_velocity = DATA_TO_COUNT(vel);
	client->writeOutputs(output);
}



void RobotJointClient::sentTorque(float torque){
	output.controlword |=  0x000f;
	output.target_torque = TORQUE_USER_TO_MOTOR(torque);
	// printf("%d\n",output.target_torque);
	client->writeOutputs(output);
}

void RobotJointClient::sentTorqueOffset(float torque){
	output.controlword |=  0x000f;
	output.torque_offset = TORQUE_USER_TO_MOTOR(torque);
	client->writeOutputs(output);
}

bool RobotJointClient::changeOPmode(uint8 opmode){

	RTIME now, previous;	/*ns */
	previous = rt_timer_read();
	int timeout = 10; 	/* 10s */

	while(input.operation_mode != opmode ){
		output.operation_mode = opmode;
		client->writeOutputs(output);
		client->servoOn();
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
	
	return float(COUNT_TO_VEL( input.velocity_actual_value) );


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

float RobotJointClient::getMotorCurrent() const{
	return CURRENT_MOTOR_TO_USER(input.current_actual_value);
}


void RobotJointClient::get_feedback(void){
	input = client->readInputs();
}


void RobotJointClient::shutdown(){

	client->reset();
	// client->servoOff();
	client->shutDown();
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

int RobotJointClient::ifReached()  {
	input = client->readInputs();
	return ((input.statusword >> 10) & 0x0001);
  }

/********************************************************************/
}// end of robot_control namespace
