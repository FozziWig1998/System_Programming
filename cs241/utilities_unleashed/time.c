/**
 * Lab: Utilities Unleashed
 * CS 241 - Fall 2017
 */
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include "format.h"
#include <string.h>

int main(int argc, char *argv[]) { 
	struct timespec t1;
	struct timespec t2;
	int status;
	pid_t id;
	if (argc <= 1) print_time_usage();


	if ((id = fork()) == 0) {
		//printf("work?!\n");
		int fail = execvp(argv[1],argv+1);
		if (fail < 0) {
			print_exec_failed();
 		}		
	} else if (id < 0) {
		print_fork_failed();
	}
	else {
		clock_gettime(CLOCK_MONOTONIC,&t1);
		waitpid(id,&status,0);
		clock_gettime(CLOCK_MONOTONIC,&t2);
		display_results(argv,(t2.tv_sec - t1.tv_sec)*1.0 + (t2.tv_nsec - t1.tv_nsec)/1000000000.0);
		
		//printf("%lf seconds\n",(t2.tv_sec - t1.tv_sec)*1.0 + (t2.tv_nsec - t1.tv_nsec)/1000000000.0 );
	}

	


	return 0; 
}
