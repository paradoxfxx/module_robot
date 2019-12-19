#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>

#include <native/task.h>
#include <native/timer.h>
#include <alchemy/mutex.h>
// #include <alchemy/task.h>
// #include <alchemy/timer.h>
// #include <alchemy/sem.h>
// #include <boilerplate/trace.h>
// #include <xenomai/init.h>

#include <stdio.h>

FILE *fp;

RT_TASK demo_task;
RT_TASK demo_task_1;

RT_MUTEX mute;
RTIME timeout;



bool stop_flag;
/* NOTE: error handling omitted. */

void demo(void *)
{
	RTIME now, previous;

	/*
	 * Arguments: &task (NULL=self),
	 *            start time,
	 *            period (here: 1s)
	 */
	rt_task_set_periodic(NULL, TM_NOW, 1000000000);
	previous = rt_timer_read();
	rt_mutex_create(&mute, "ethercat_manager");

	
	while (1) {
		rt_task_wait_period(NULL);
		now = rt_timer_read();

		/*
		 * NOTE: printf may have unexpected impact on the timing of
		 *       your program. It is used here in the critical loop
		 *       only for demonstration purposes.
		 */
		// rt_task_sleep(100000);  // 100 us

		rt_mutex_acquire(&mute,timeout);
		printf("Time since last turn: %ld.%06ld us\n",
		       (long)(now - previous) / 1000,
		       (long)(now - previous) % 1000);
		       previous = now;

		fp = fopen("./test.txt", "w+");
   		fputs("This is testing for fputs...\n", fp);


		fclose(fp);
		rt_mutex_release(&mute);
	}
	
}


void demo_2(void *stop_flag){

	rt_task_set_periodic(NULL, TM_NOW, 500000000);
	RT_MUTEX_INFO info;
	int ret;
	RTIME now, previous;

	previous = rt_timer_read();

	while(! stop_flag){

		rt_task_wait_period(NULL);

		ret = rt_mutex_inquire(&mute, &info);

		printf("mute: %s\n",ret ? "false":"true");

		if (ret == 0){
			rt_mutex_acquire(&mute,timeout);

			fp = fopen("./test.txt", "w+");

			now = rt_timer_read();
			fclose(fp);

			printf("time pass : %.4f ms \n", (now - previous) / 1000000.0);
			

			rt_mutex_release(&mute);
			
		}
	}
	printf("stop demo_2 task!!!\n");

}

void catch_signal(int sig)
{	
	stop_flag = true;
	rt_task_sleep(3000000000);  // 3s
	rt_mutex_delete(&mute);

	rt_task_delete(&demo_task);
	rt_task_delete(&demo_task_1);
	printf("delete task & mutex....\n");
}

int main(int argc, char* argv[])

{
	signal(SIGINT, catch_signal);
	int success;
	RTIME timeout = 2000000000;  // 2s
	stop_flag = false;
	FILE *fp = NULL;
	/* Avoids memory swapping for this program */
	mlockall(MCL_CURRENT|MCL_FUTURE);

	/*
	 * Arguments: &task,
	 *            name,
	 *            stack size (0=default),
	 *            priority,
	 *            mode (FPU, start suspended, ...)
	 */
	success = rt_task_create(&demo_task, "trivial", 0, 13, 0);
	rt_printf("task ceate: %s\n",success ? "false" : "success");
	success = rt_task_create(&demo_task_1, "trivial_1", 0, 12, 0);
	rt_printf("task ceate: %s\n",success ? "false" : "success");

	/*
	 * Arguments: &task,
	 *            task function,
	 *            function argument
	 */
	success = rt_task_start(&demo_task, &demo, (void * )NULL);
	rt_printf("task start: %s\n",success ? "false" : "success");
	success = rt_task_start(&demo_task_1, &demo_2,  (void *)stop_flag);
	rt_printf("task start: %s\n",success ? "false" : "success");
	pause();



	return 0;
}
