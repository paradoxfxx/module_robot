#include <ethercat_manager/ethercat_manager.h>

#include <stdexcept>
#include <signal.h>
#include <sys/mman.h>

#define TASK_PRIO  99              /* Highest RT priority */
#define TASK_MODE  0  /* Uses FPU, bound to CPU #0 */
#define TASK_STKSZ 4096            /* Stack size (in bytes) */

RT_TASK motion_task;

int run = 1;


void catch_signal(int sig)
{
    run = 0;
	usleep(1e5);
	rt_task_delete(&motion_task);
	exit(1);
}

void ethercat_run(void *arg){

	RTIME period = 1e+6 ; //1ms
	RTIME start;
	RTIME end;
	RTIME error;

	int index = 0;
	RTIME temp = 0;
	error = 0;

	while(run){

		start = rt_timer_read();
		std::cout<<"start: "<<start<<" ns"<<std::endl;

		rt_task_sleep(period);

		end = rt_timer_read();
		std::cout<<"end: "<<end<<" ns"<<std::endl;

		temp = end - period - start;
		std::cout<<"error: "<<temp<<std::endl;

		error += temp;
		index += 1;		
	}

	std::cout<<"\n\nindex: "<<index<<std::endl;
	std::cout<<"average time delay: "<<error/index<<"ns\n";
}

int main(int argc, char *argv[]){

	signal(SIGTERM, catch_signal);
	signal(SIGINT, catch_signal);
	mlockall(MCL_CURRENT | MCL_FUTURE);

	int err;

    err = rt_task_create(&motion_task,
                         "SOEM_motion_task",
                         TASK_STKSZ,
                         TASK_PRIO,
                         TASK_MODE); 

	if(! err){	

		rt_printf("task start!!\n");
		rt_task_start(&motion_task, &ethercat_run, NULL);
	}else{
		return 0;
	}

    while(run){
        usleep(1000);
    }

    return 0;
}

