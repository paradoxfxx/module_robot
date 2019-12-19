#include <elmo_control/elmo_client.h>
#include <ethercat_manager/ethercat_manager.h>
#include "common.h"


int main(int argc, char const *argv[])
{
    bool real_time = true;
    int slave_no = 1;

    elmo_control::ElmoInput input;
    elmo_control::ElmoOutput output;

    ethercat::EtherCatManager manager(NETWORK_CARD_NAME,true);
    elmo_control::ElmoClient * client;
    client = new elmo_control::ElmoClient( manager, slave_no);


    printf("Initialize EtherCATJoint (reset)\n");
    client->reset();

    printf("Initialize EtherCATJoint (InterpolationTimePeriod)\n");
    client->setInterpolationTimePeriod(DEFAULT_INTERPOLATION_TIME_PERIOD / 1000);   /* (/1000): ms */

    client->setMaxVelocity(1000); 
    printf("Set motor rate torque %.2f mN.m\n",RATE_TORQUE);
    client->setMotorRateTorque(RATE_TORQUE);

	printf("Set motor max torque %.2f mN.m\n",MAX_TORQUE / RATE_TORQUE * 1000);
    client->setMaxTorque(MAX_TORQUE);
    // servo on
    printf("Initialize EtherCATJoint (servoOn)\n");
    client->servoOn();

    printf("Initialize EtherCATJoint (readInputs)\n");
	memset(&input, 0x00, sizeof(elmo_control::ElmoInput));
    input = client->readInputs();
    memset(&output, 0x00, sizeof(elmo_control::ElmoOutput));
    output.target_position  = input.position_actual_value;

   
    output.max_torque    = 0x2220;    

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

    uint8 opmode = 10;

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
	}else{
		fprintf(stderr,"Set operation mode error...");
		return -1;
	}

    for ( int i = 0 ; i < 20000; i++)
    {
    	input = client->readInputs();
        output.controlword |= 0x0f;
        // output.target_position = input.position_actual_value + DATA_TO_COUNT(5);
        output.target_torque = TORQUE_USER_TO_MOTOR(200);
        // output.target_torque = 50;

        // printf("%04x\n",output.target_torque);
        client->writeOutputs(output);


        if(i % 100 == 0){
            printf("pos: %.2f deg !.\n" , COUNT_TO_DATA(input.position_actual_value));
			printf("vel: %.2f deg/s !.\n" , COUNT_TO_DATA(input.velocity_actual_value));
			printf("torque: %f mN.m !.\n",TORQUE_MOTOR_TO_USER(input.torque_actual_value));
			printf("status word %04x .\n", input.statusword);
        }
        rt_timer_spin(DEFAULT_INTERPOLATION_TIME_PERIOD);


    }

    delete client;


    return 0;
}
