#include <robot_control/robot_joint_client.h>

int main(int argc, char *argv[]){
	

    ethercat::EtherCatManager manager(NETWORK_CARD_NAME);
	
	robot_control::RobotJointClient* joint1;
    joint1 = new robot_control::RobotJointClient (manager , 1);

	for(int i = 0; i < 10000; i++){
		
        printf("actual chip temperature(celsius): %d \n",joint1->getChipTemp());

		rt_timer_spin(1e9); /*1s */

	}

	delete joint1;

	return 0;
}


