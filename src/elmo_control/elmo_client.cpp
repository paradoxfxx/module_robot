#include <stdio.h>
#include <elmo_control/elmo_client.h>

// An effort to keep the lines less than 100 char long
namespace elmo_control
{
static const unsigned SLEEP_TIME_MS = 1; // 1 ms

ElmoClient::ElmoClient(ethercat::EtherCatManager& manager, int slave_no)
  : manager_(manager)
  , slave_no_(slave_no)
{}

void ElmoClient::writeOutputs(const ElmoOutput& output)
{
	uint8_t map[25] = {0}; 
	map[0] = (output.target_position) & 0x00ff;
	map[1] = (output.target_position >> 8) & 0x00ff;
	map[2] = (output.target_position >> 16) & 0x00ff;
	map[3] = (output.target_position >> 24) & 0x00ff;

	map[4] = (output.target_velocity) & 0x00ff;
	map[5] = (output.target_velocity >>  8) & 0x00ff;
	map[6] = (output.target_velocity >> 16) & 0x00ff;
	map[7] = (output.target_velocity >> 24) & 0x00ff;

	map[8] = (output.target_torque) & 0x00ff; 
	map[9] = (output.target_torque >>  8) & 0x00ff;

	map[10] = (output.max_torque) & 0x00ff; 
	map[11] = (output.max_torque >>  8) & 0x00ff;

	map[12] = (output.controlword) & 0x00ff;
	map[13] = (output.controlword >> 8) & 0x00ff;

	map[14] = (output.operation_mode) & 0xff;

	map[15] = (output.position_offset) & 0x00ff;
	map[16] = (output.position_offset >>  8) & 0x00ff;
	map[17] = (output.position_offset >> 16) & 0x00ff;
	map[18] = (output.position_offset >> 24) & 0x00ff;

	map[19] = (output.velocity_offset) & 0x00ff;
	map[20] = (output.velocity_offset >>  8) & 0x00ff;
	map[21] = (output.velocity_offset >> 16) & 0x00ff;
	map[22] = (output.velocity_offset >> 24) & 0x00ff;

	map[23] = (output.torque_offset) & 0x00ff;
	map[24] = (output.torque_offset >>  8) & 0x00ff;

	for (unsigned i = 0; i < 25; ++i)
	{
		manager_.write(slave_no_, i, map[i]);
	}
}

ElmoInput ElmoClient::readInputs() const
{
	ElmoInput input;
	uint8_t map[27];
	for (unsigned i = 0; i < 27; ++i)
	{
		map[i] = manager_.readInput(slave_no_, i);
	}

	input.position_actual_value		    = *(uint32 *)(map+0);
	input.position_follow_error_value 	= *(uint32 *)(map+4);
	input.torque_actual_value		    = *(uint16 *)(map+8);
	input.statusword			        = *(uint16 *)(map+10);
	input.operation_mode			    = *(uint8  *)(map+12);
	input.velocity_sensor_actual_value	= *(uint32 *)(map+13);
	input.velocity_actual_value		    = *(uint32 *)(map+17);
	input.digital_inputs			    = *(uint32 *)(map+21);
	input.analog_input                	= *(uint16 *)(map+25);
	// input.current_actual_value        	= *(uint32 *)(map+27);
	return input;
}

ElmoOutput ElmoClient::readOutputs() const
{

	ElmoOutput output;

	uint8_t map[25];
	for (unsigned i = 0; i < 25; ++i)
	{
		map[i] = manager_.readOutput(slave_no_, i);
	}
	output.target_position	= *(uint32 *)(map+0);
	output.target_velocity  = *(uint32 *)(map+4);
	output.target_torque	= *(uint16 *)(map+8);
	output.max_torque     	= *(uint16 *)(map+10);
	output.controlword  	= *(uint16 *)(map+12);
	output.operation_mode	= *(uint8  *)(map+14);
	output.position_offset	= *(uint16 *)(map+15);
	output.velocity_offset  = *(uint16 *)(map+19);
	output.torque_offset	= *(uint16 *)(map+23);

	return output;
}

void ElmoClient::reset()
{
	ElmoOutput output;
	memset(&output, 0x00, sizeof(ElmoOutput));
	output.controlword = 0x0080; // fault reset
	output.operation_mode = 0x08; // csp mode
	writeOutputs(output);
	printf("Fault was cleared\n");
}

void ElmoClient::servoOn()
{
	ElmoInput input = readInputs();
	printPDSStatus(input);
	ElmoOutput output;
	memset(&output, 0x00, sizeof(ElmoOutput));
	int loop = 0;
	while (getPDSStatus(input) != OPERATION_ENABLED) {
		switch ( getPDSStatus(input) ) {
			case SWITCH_DISABLED:
				output.controlword = 0x0006; // move to ready to switch on
				break;
			case READY_SWITCH:
				output.controlword = 0x0007; // move to switched on
				break;
			case SWITCHED_ON:
				output.controlword = 0x000f; // move to operation enabled
				break;
			case OPERATION_ENABLED:
				break;
			default:
				printf("unknown status");
				return;
		}
		writeOutputs(output);
		rt_timer_spin(DEFAULT_INTERPOLATION_TIME_PERIOD);
		input = readInputs();
		if (loop++ % 1000 == 1) printPDSStatus(input);
	}
}

void ElmoClient::servoOff()
{
	ElmoInput input = readInputs();
	printPDSStatus(input);
	ElmoOutput output;
	memset(&output, 0x00, sizeof(ElmoOutput));
	int loop = 0;
	while (getPDSStatus(input) != SWITCH_DISABLED) {
		switch ( getPDSStatus(input) ) {
			case READY_SWITCH:
				output.controlword = 0x0000; // disable voltage
				break;
			case SWITCHED_ON:
				output.controlword = 0x0006; // shutdown
				break;
			case OPERATION_ENABLED:
				output.controlword = 0x0007; // disable operation
				break;
			default:
				printf("unknown status");
				output.controlword = 0x0000; // disable operation
				break;
		}
		writeOutputs(output);
		usleep(SLEEP_TIME_MS*1000);
		input = readInputs();
		if (loop++ % 100 == 1) printPDSStatus(input);
	}
}

PDS_STATUS ElmoClient::getPDSStatus(const ElmoInput input) const
{
	uint16 statusword = input.statusword;
	// printf("statusword: %4x \n",statusword );
	if (((statusword) & 0x004f) == 0x0000) { // x0xx 0000
		return NOT_READY;
	}else if (((statusword) & 0x004f) == 0x0040) { // x1xx 0000
		return SWITCH_DISABLED;
	}else if (((statusword) & 0x006f) == 0x0021) { // x01x 0001
		return READY_SWITCH;
	}else if (((statusword) & 0x006f) == 0x0023) { // x01x 0011
		return SWITCHED_ON;
	}else if (((statusword) & 0x006f) == 0x0027) { // x01x 0111
		return OPERATION_ENABLED;
	}else if (((statusword) & 0x006f) == 0x0007) { // x00x 0111
		return QUICK_STOP;
	}else if (((statusword) & 0x004f) == 0x000f) { // x0xx 1111
		return FAULT_REACTION;
	}else if (((statusword) & 0x004f) == 0x0008) { // x0xx 1000
		return FAULT;
	}else{
		return UNKNOWN;
  }
}

PDS_OPERATION ElmoClient::getPDSOperation(const ElmoInput input) const
{
	int8 operation_mode = input.operation_mode;
	switch (operation_mode) {
	case 0: return NO_MODE_CHANGE_1;	break;
	case 1: return PROFILE_POSITION_MODE;	break; // pp
	case 2: return VELOCITY_MODE;		break; // vl
	case 3: return PROFILE_VELOCITY_MODE;	break; // pv
	case 4: return TORQUE_PROFILE_MODE;	break; // tq
	case 5: return NO_MODE_CHANGE_2;		break; // hm
	case 6: return HOMING_MODE;		break; // hm
	case 7: return INTERPOLATED_POSITION_MODE;	break; // ip
	case 8: return CYCLIC_SYNCHRONOUS_POSITION_MODE;	break; // csp
	case 9: return CYCLIC_SYNCHRONOUS_VELOCITY_MODE;	break; // csv
	case 10: return CYCLIC_SYNCHRONOUS_TORQUE_MODE;	break; // cst
	}
}

PDS_STATUS ElmoClient::getPDSControl(const ElmoInput input) const
{
	// uint16 statusword = input.statusword;
}



void ElmoClient::printPDSStatus(const ElmoInput input) const
{
	printf("Statusword(6041h): %04x\n ", input.statusword);
	switch ( getPDSStatus(input) ) {
	case NOT_READY:
		printf("Not ready to switch on\n");
		break;
	case SWITCH_DISABLED:
		printf("Switch on disabled\n");
		break;
	case READY_SWITCH:
		printf("Ready to switch on\n");
		break;
	case SWITCHED_ON:
		printf("Switched on\n");
		break;
	case OPERATION_ENABLED:
		printf("Operation enabled\n");
		break;
	case QUICK_STOP:
		printf("Quick stop active\n");
		break;
	case FAULT_REACTION:
		printf("Fault reaction active\n");
		break;
	case FAULT:
		printf("Fault\n");
		break;
	case UNKNOWN:
		printf("Unknown status %04x\n", input.statusword);
		break;
	}
	if ( input.statusword & 0x0800 ) {
		printf(" Internal limit active\n");
	}


	switch ( getPDSOperation(input) ) {
		case PROFILE_POSITION_MODE:
			if ( (input.statusword & 0x3400) | 0x2000 ) {
				printf(" Following error\n");
			}
			if ( (input.statusword & 0x3400) | 0x1000 ) {
				printf(" Set-point acknowledge\n");
			}
			if ( (input.statusword & 0x3400) | 0x0400 ) {
				printf(" Target reached\n");
			}
			break;
		case VELOCITY_MODE:
			break;
		case PROFILE_VELOCITY_MODE:
			if ( (input.statusword & 0x3400) | 0x2000 ) {
				printf(" Max slippage error (Not supported)\n");
			}
			if ( (input.statusword & 0x3400) | 0x1000 ) {
				printf(" Speed\n");
			}
			if ( (input.statusword & 0x3400) | 0x0400 ) {
				printf(" Target reached\n");
			}
			break;
		case TORQUE_PROFILE_MODE:
			if ( (input.statusword & 0x3400) | 0x0400 ) {
				printf(" Target reached\n");
			}
			break;
		case HOMING_MODE:
			if ( (input.statusword & 0x3400) | 0x2000 ) {
				printf(" Homing error\n");
			}
			if ( (input.statusword & 0x3400) | 0x1000 ) {
				printf(" Homing attained\n");
			}
			if ( (input.statusword & 0x3400) | 0x0400 ) {
				printf(" Target reached\n");
			}
			break;
		case INTERPOLATED_POSITION_MODE:
			if ( (input.statusword & 0x3400) | 0x1000 ) {
				printf(" Ip mode active\n");
			}
			if ( (input.statusword & 0x3400) | 0x0400 ) {
				printf(" Target reached\n");
			}
			break;
		case CYCLIC_SYNCHRONOUS_POSITION_MODE:
			if ( (input.statusword & 0x3400) | 0x2000 ) {
				printf(" Following error\n");
			}
			if ( (input.statusword & 0x3400) | 0x1000 ) {
				printf(" Drive follows command value\n");
			}
			break;
		case CYCLIC_SYNCHRONOUS_VELOCITY_MODE:
			if ( (input.statusword & 0x3400) | 0x1000 ) {
				printf(" Drive follows command value\n");
			}
			break;
			case CYCLIC_SYNCHRONOUS_TORQUE_MODE:
			if ( (input.statusword & 0x3400) | 0x1000 ) {
				printf(" Drive follows command value\n");
			}
			break;
	}
}

void ElmoClient::printPDSOperation(const ElmoInput input) const
{
	printf("Mode of operation(6061h): %04x\n ", input.operation_mode);
	switch ( getPDSOperation(input) ) {
		case NO_MODE_CHANGE_1:
			printf("No mode change / no mode assigned\n");
			break;
		case PROFILE_POSITION_MODE:
			printf("Profile position mode\n");
			break;
		case VELOCITY_MODE:
			printf("Velocity mode\n");
			break;
		case PROFILE_VELOCITY_MODE:
			printf("Profile velocity mode\n");
			break;
		case TORQUE_PROFILE_MODE:
			printf("Torque profile mode\n");
			break;
		case NO_MODE_CHANGE_2:
			printf("No mode change / no mode assigned\n");
			break;
		case HOMING_MODE:
			printf("Homing mode\n");
			break;
		case INTERPOLATED_POSITION_MODE:
			printf("Interpolated position mode\n");
			break;
		case CYCLIC_SYNCHRONOUS_POSITION_MODE:
			printf("Cyclic synchronous position mode\n");
			break;
		case CYCLIC_SYNCHRONOUS_VELOCITY_MODE:
			printf("Cyclic synchronous velocity mode\n");
			break;
		case CYCLIC_SYNCHRONOUS_TORQUE_MODE:
			printf("Cyclic synchronous torque mode\n");
			break;
		default:
			printf("Reserved %04x\n", input.operation_mode);
			break;
	}
}

void ElmoClient::printPDSControl(const ElmoInput input) const
{
	
}

int ElmoClient::setProfileVelocity(uint32_t val)
{
	// 6091h, unit: pulse, range: 0 - 4294967295, U32
	uint32_t u32val = (uint32_t)val;
	manager_.writeSDO<uint32_t>(slave_no_, 0x6081, 0x00, u32val);
	int ret = 0 ;
	uint32_t temp;
	 int timeout = 10;
	RTIME now, previous;	/*ns */
	previous = rt_timer_read(); 
	while(temp != u32val){
		printf("setProfileVelocity...\n");
		manager_.writeSDO<uint32_t>(slave_no_, 0x6081, 0x00, u32val);
		temp = manager_.readSDO<uint32_t>(slave_no_,0x6081,0x00);
		rt_timer_spin(DEFAULT_INTERPOLATION_TIME_PERIOD);  
		if((now - previous) / 1e9 >= timeout ){	
			ret = -1;
			break;
		} 
	}
	return ret;
}

int ElmoClient::setMaxVelocity(uint32_t max_velocity)
{
	uint32_t u32val = (uint32_t)max_velocity;
	manager_.writeSDO<uint32_t>(slave_no_, 0x6080, 0x00, u32val);
	int ret = 0 ;
	uint32_t temp;
	 int timeout = 10;
	RTIME now, previous;	/*ns */
	previous = rt_timer_read(); 
	while(temp != u32val){
		printf("setMaxVelocity...\n");
		manager_.writeSDO<uint32_t>(slave_no_, 0x6080, 0x00, u32val);
		temp = manager_.readSDO<uint32_t>(slave_no_,0x6080,0x00);
		rt_timer_spin(DEFAULT_INTERPOLATION_TIME_PERIOD);  
		if((now - previous) / 1e9 >= timeout ){	
			ret = -1;
			break;
		} 
	}
	return ret;
}

int ElmoClient::setMotorRateTorque(uint32_t rate_torque)
{

    // 6076h, unit: pulse, range: 0 - 4294967295, U32
    uint32_t u32val = (uint32_t)rate_torque;
    manager_.writeSDO<uint32_t>(slave_no_, 0x6076, 0x00, u32val);
	int ret = 0 ;
	uint32_t temp;
	int timeout = 10;
	RTIME now, previous;	/*ns */
	previous = rt_timer_read(); 
	while(temp != u32val){
		printf("setMotorRateTorque...\n");
		manager_.writeSDO<uint32_t>(slave_no_, 0x6076, 0x00, u32val);
		temp = manager_.readSDO<uint32_t>(slave_no_,0x6076,0x00);
		rt_timer_spin(DEFAULT_INTERPOLATION_TIME_PERIOD);  
		if((now - previous) / 1e9 >= timeout ){	
			ret = -1;
			break;
		} 
	}
	return ret;
}

int ElmoClient::setMaxTorque(uint16_t max_torque){

    uint16_t u16val = (uint16_t)max_torque;
    manager_.writeSDO<uint16_t>(slave_no_, 0x6072, 0x00, u16val);
	int ret = 0 ;
	uint16_t temp;
	 int timeout = 10;
	RTIME now, previous;	/*ns */
	previous = rt_timer_read(); 
	while(temp != u16val){
		printf("setMaxTorque...\n");
		manager_.writeSDO<uint16_t>(slave_no_, 0x6072, 0x00, u16val);
		temp = manager_.readSDO<uint16_t>(slave_no_,0x6072,0x00);
		rt_timer_spin(DEFAULT_INTERPOLATION_TIME_PERIOD);  
		if((now - previous) / 1e9 >= timeout ){	
			ret = -1;
			break;
		} 
	}
	return ret;
}

uint32_t ElmoClient::readMotorRateTorque()const
{
    return (manager_.readSDO<uint32_t>(slave_no_,0x6076,0x00));
}

uint32_t ElmoClient::getAbsolutePos()const{
    return (manager_.readSDO<uint32_t>(slave_no_,0x3091,0x02));
}

uint32_t ElmoClient::getAbsoluteVel()const{
	return (manager_.readSDO<uint32_t>(slave_no_,0x3097,0x02));
}

void ElmoClient::setInterpolationTimePeriod(int us)
{
	uint32_t u32val;	/*time unit : ns */
	uint8_t u8val;
	switch ( us ) {
		case 1000: u32val = 1000000; 	u8val = 1; break;
		case 2000: u32val = 2000000; 	u8val = 2; break;
		case 4000: u32val = 4000000; 	u8val = 4; break;
		default:
			fprintf(stderr, "setInterpolatinTimePeriod(%d) must be ether of 1000, 2000, 4000\n", us);
		return;
	}
	interpolationTimePeriod = u8val;
	int ret = 0;
	ret += manager_.writeSDO<uint32_t>(slave_no_, 0x1c32, 0x02, u32val);
	ret += manager_.writeSDO<uint8_t>(slave_no_, 0x60c2, 0x01, u8val);
	printf("Set interpolation time period %d us\n", us);

	u32val = manager_.readSDO<uint32_t>(slave_no_, 0x1c32, 0x02);
	u8val = manager_.readSDO<uint8_t>(slave_no_, 0x60c2, 0x01);
	printf("1c32h: cycle time %04x\n", u32val);
	printf("60c2h: interpolation time period value %02d\n", u8val);

}

int ElmoClient::getInterpolationTimePeriod(){
	return interpolationTimePeriod;
}


uint32_t ElmoClient::readTrajBuffSize() const
{
	uint32_t ret;
	ret = manager_.readSDO<uint32_t>(slave_no_, 0x60c4, 0x2);
	return ret;
}

uint16_t ElmoClient::readActualBuffPos() const
{
	uint16_t ret;
	ret = manager_.readSDO<uint16_t>(slave_no_, 0x60c4, 0x4);
	return ret;
}

  int ElmoClient::setAcc(uint32_t u32val){

    manager_.writeSDO<uint32_t>(slave_no_, 0x6083, 0x00, u32val);
	int ret = 0 ;
	uint32_t temp;
	int timeout = 10;
	RTIME now, previous;	/*ns */
	previous = rt_timer_read(); 
	while(temp != u32val){
		printf("setAcc...\n");
		manager_.writeSDO<uint32_t>(slave_no_, 0x6083, 0x00, u32val);
		temp = manager_.readSDO<uint32_t>(slave_no_,0x6083,0x00);
		rt_timer_spin(DEFAULT_INTERPOLATION_TIME_PERIOD);  
		if((now - previous) / 1e9 >= timeout ){	
			ret = -1;
			break;
		} 
	}
	return ret;

  }

  int ElmoClient::setDcc(uint32_t u32val){

	manager_.writeSDO<uint32_t>(slave_no_, 0x6084, 0x00, u32val);
	int ret = 0 ;
	uint32_t temp;
	int timeout = 10;
	RTIME now, previous;	/*ns */
	previous = rt_timer_read(); 
	while(temp != u32val){
		printf("setAcc...\n");
		manager_.writeSDO<uint32_t>(slave_no_, 0x6084, 0x00, u32val);
		temp = manager_.readSDO<uint32_t>(slave_no_,0x6084,0x00);
		rt_timer_spin(DEFAULT_INTERPOLATION_TIME_PERIOD);  
		if((now - previous) / 1e9 >= timeout ){	
			ret = -1;
			break;
		} 
	}
	return ret;
  }

  int16_t ElmoClient::readChipTemp(){
	int16_t ret;
	ret = manager_.readSDO<uint16_t>(slave_no_, 0x22a3, 0x1);
	return ret;
  }


} // end of elmo_control namespace
