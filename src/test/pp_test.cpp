#include <robot_control/robot_joint_client.h>

int main(int argc, char *argv[]){

    ethercat::EtherCatManager manager(NETWORK_CARD_NAME);
	
	robot_control::RobotJointClient* joint1;
    joint1 = new robot_control::RobotJointClient (manager , 1);

	joint1->changeOPmode(PROFILE_POSITION_MODE);
	joint1->get_feedback();


	float pos = joint1->getMotorPos();
	printf("pos: %.2f\n",pos);
	float position = 5000 + pos;
	joint1->sentPos(position,1000);

	int i = 0;
	while(! joint1->ifReached()){
			
		joint1->get_feedback();

		if(i%300 == 0 ){
			printf("pos: %.2f deg !.\n" , joint1->getMotorPos());
			printf("vel: %.2f deg/s !.\n" , joint1->getMotorVel());
			printf("torque: %f mN.m !.\n",joint1->getMotorTorque());
			printf("status word %04x .\n", joint1->getStatusWord());
			printf("op mode  %d .\n", joint1->readOpmode());

		}

		i+= 1;
		rt_timer_spin(DEFAULT_INTERPOLATION_TIME_PERIOD);
		
	}

	pos = joint1->getMotorPos();
	
	position = 5000 + pos;
	joint1->sentPos(position,1500);
	i = 0;
	rt_timer_spin(DEFAULT_INTERPOLATION_TIME_PERIOD * 1000);

	while(! joint1->ifReached()){
		
		joint1->get_feedback();

		if(i%100 == 0 ){
			printf("pos: %.2f deg !.\n" , joint1->getMotorPos());
			printf("vel: %.2f deg/s !.\n" , joint1->getMotorVel());
			printf("torque: %f mN.m !.\n",joint1->getMotorTorque());
			printf("status word %04x .\n", joint1->getStatusWord());
			printf("op mode  %d .\n", joint1->readOpmode());

		}

		i+= 1;
		rt_timer_spin(DEFAULT_INTERPOLATION_TIME_PERIOD);

	}

	joint1->shutdown();
	delete joint1;

	return 0;
}


