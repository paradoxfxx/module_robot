#include "robot_control/robot_client.h"

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
	
	robot_control::RobotClient* joint1;
    joint1 = new robot_control::RobotClient (manager , 1);

	joint1->changeOPmode(CYCLIC_SYNCHRONOUS_TORQUE_MODE);

	double period = 3e+5;  //300us
	struct timespec tick;
	clock_gettime(CLOCK_REALTIME, &tick);
	timespecInc(tick, period);


	for(size_t i = 0; i < 2000; i++){
		
		joint1->sentorque(200); //500mN.m

		if(i%10 == 0 ){

			joint1->getfeedback();
			ROS_INFO("pos: %f deg !.\n" , joint1->getpos());
			ROS_INFO("vel: %f deg/s !.\n" , joint1->getvel());
			ROS_INFO("torque: %f mN.m !.\n",joint1->getorque());
			
		}

		clock_gettime(CLOCK_REALTIME, &tick);
		timespecInc(tick, period);
	}

	delete joint1;

	return 0;
}


