#include "pied_piper.h"
#include "utils.h"
#include <fcntl.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define TRIES 3

char error_str[100][1000];
int err_st[100];

void clear_err_msg(int num_cmds) {
	for (int i = 0; i < num_cmds; i++) {
		memset(error_str[i], 0, 1000);
	}
}

int pied_piper(int input_fd, int output_fd, char **executables) {
    // Your code goes here...
	if (input_fd == -1) {
		input_fd = 0;
	}
	if (output_fd == -1) {
		output_fd = 1;
	}

	int num_cmds = 0;

	for (char** it = executables; *it != NULL; it++) {
		num_cmds++;
	}

	int flag_print = 0;
	int count = 0;

	pid_t child[100];
RESTART:
	clear_err_msg(num_cmds);
	reset_file(output_fd);
	reset_file(input_fd);
	for (int i = 0; i < num_cmds; i++) {
		int pp[2];
		int pp_err[2];
		pipe(pp);
		pipe(pp_err);
		char* this_cmd = executables[i];
		child[i] = fork();
		if (child[i] == 0) {
			// child program
			if (i == 0) dup2(input_fd, 0);
			close(pp[0]);
			if (i == num_cmds - 1) {
				dup2(output_fd, 1);
			} else {
				dup2(pp[1], 1); //stdout replaced by pipe
			}
			close(pp_err[0]);
			dup2(pp_err[1], 2);
    		exec_command(this_cmd);
    		fprintf(stderr, "EXEC FAIL!!\n");
    		exit(-1);
		} else {
			int st;
    		waitpid(child[i], &st, 0);
    		err_st[i] = st;

			close(pp[1]);
    		dup2(pp[0], 0);
    		
    		close(pp_err[1]);
    		read(pp_err[0], error_str[i], 1000);
		}
	}
	for (int i = 0; i < num_cmds; i++) {
		if (WEXITSTATUS(err_st[i]) != 0) {
			count++;
			if (count == 3)	{
				flag_print = 1;
				break;
			} else {
    			goto RESTART;
			}		
		}
	}

	if (flag_print) {
		failure_information info[100];
		for (int i = 0; i < num_cmds; i++) {
			info[i].command = executables[i];
			info[i].status = err_st[i];
			info[i].error_message = error_str[i];
		}
		print_failure_report(info, num_cmds);
    	exit(5);
    }

    return EXIT_SUCCESS;
}	