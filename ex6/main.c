#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>

#include <native/task.h>
#include <native/timer.h>

#include  <rtdk.h>

#include <sys/io.h>
#include "io.h"

void funksjon()
{
	printf("hoho\n");
}


void periodicPrint()
{
	rt_task_set_periodic(NULL, TM_NOW, 1000000000);
	
	while(1)
	{
		printf("Maja-hi, Maja-ho, Maja-ha, Maja-ha-ha\n");
		
		rt_task_wait_period(NULL);
	}
}

/*

void* disturbance() {
	set_cpu(CPU_CORE);
	double i, j, k;
	while(1) {
		i++;
		j++;
		k = i*j;
		k++;
	}
}

*/


void catch_signal(int sig) {
}


void wait_for_ctrl_c() {
	signal(SIGTERM, catch_signal);
	signal(SIGINT, catch_signal);
  // wait for SIGINT (CTRL-C) or SIGTERM signal
	pause();
	printf("     HADE :D\n");
}




void periodicTest(void* num_param) {
	int num = (long) num_param;
	io_write(num, 1);
	rt_task_set_periodic(NULL, TM_NOW, 1000000);
	
	while(1) {
		while (io_read(num));
		printf("hei fra periodic task\n");
		io_write(num, 0);
		rt_task_wait_period(NULL);
		io_write(num, 1);
	}
}




int main ()
{
	RT_TASK test[3];
	mlockall(MCL_CURRENT|MCL_FUTURE);
	
	long i;
	for (i = 1; i <= 3; i++) {
    	rt_task_create(&test[i-1], (char*) i, 0, 99, T_CPU(1)|T_JOINABLE);
    	rt_task_start(&test[i-1], &periodicPrint, (void*) i);
    }
    
   
    
    /*
	pthread_t disturbances[10];
    
    for (i = 0; i < 10; i++) {
    	pthread_create(&disturbances[i], NULL, disturbance, NULL);
    }
	for (i = 0; i < 10; i++) {
		pthread_join(disturbances[i], NULL);
	}
	*/
	wait_for_ctrl_c();
	
	return 0;
}
