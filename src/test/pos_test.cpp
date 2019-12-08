#include <robot_control/robot_joint_client.h>

static const int NSEC_PER_SECOND = 1e+9;


void timespecInc(struct timespec &tick, int nsec)
{
    tick.tv_nsec += nsec;
    while (tick.tv_nsec >= NSEC_PER_SECOND)
    {
        tick.tv_nsec -= NSEC_PER_SECOND;
        tick.tv_sec++;
    }
}


int main(int argc, char *argv[]){

    ethercat::EtherCatManager manager(NETWORK_CARD_NAME);
	
	robot_control::RobotJointClient* joint1;
    joint1 = new robot_control::RobotJointClient (manager , 1);

	joint1->changeOPmode(CYCLIC_SYNCHRONOUS_POSITION_MODE);

	double period = 3e+5;  //300us
	struct timespec tick;
	clock_gettime(CLOCK_REALTIME, &tick);
	timespecInc(tick, period);


	for(size_t i = 0; i < 20000; i++){
		

		if(i%10 == 0 ){

			printf("pos: %f deg !.\n" , joint1->getMotorPos());
			printf("vel: %f deg/s !.\n" , joint1->getMotorVel());
			printf("torque: %f mN.m !.\n",joint1->getMotorTorque());
			
		}

		clock_gettime(CLOCK_REALTIME, &tick);
		timespecInc(tick, period);
        usleep(1000);
	}

	delete joint1;

	return 0;
}


