#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>

#include <native/task.h>
#include <native/timer.h>

#include <alchemy/task.h>
#include <alchemy/timer.h>
#include <alchemy/sem.h>
#include <boilerplate/trace.h>
#include <xenomai/init.h>

RT_TASK demo_task;

/* NOTE: error handling omitted. */

void demo(void *arg)
{
	RTIME now, previous;

	/*
	 * Arguments: &task (NULL=self),
	 *            start time,
	 *            period (here: 1 s)
	 */
	rt_task_set_periodic(NULL, TM_NOW, 1000000000);
	previous = rt_timer_read();

	while (1) {
		rt_task_wait_period(NULL);
		now = rt_timer_read();

		/*
		 * NOTE: printf may have unexpected impact on the timing of
		 *       your program. It is used here in the critical loop
		 *       only for demonstration purposes.
		 */
		printf("Time since last turn: %ld.%06ld ms\n",
		       (long)(now - previous) / 1000000,
		       (long)(now - previous) % 1000000);
		       previous = now;
	}
}

void catch_signal(int sig)
{
}

int main(int argc, char* argv[])
{
	signal(SIGTERM, catch_signal);
	signal(SIGINT, catch_signal);
	int success;

	/* Avoids memory swapping for this program */
	mlockall(MCL_CURRENT|MCL_FUTURE);

	/*
	 * Arguments: &task,
	 *            name,
	 *            stack size (0=default),
	 *            priority,
	 *            mode (FPU, start suspended, ...)
	 */
	success = rt_task_create(&demo_task, "trivial", 0, 99, 0);
	rt_printf("%d\n",success);
	/*
	 * Arguments: &task,
	 *            task function,
	 *            function argument
	 */
	success = rt_task_start(&demo_task, &demo, NULL);
	rt_printf("%d\n",success);

	pause();

	rt_task_delete(&demo_task);

	return 0;
}
