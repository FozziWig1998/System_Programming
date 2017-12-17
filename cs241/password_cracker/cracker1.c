/**
 * Machine Problem: Password Cracker
 * CS 241 - Fall 2017
 */

#include "cracker1.h"
#include "format.h"
#include "utils.h"
#include "queue.h"
#include <unistd.h>
#include <crypt.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <math.h>

char name[50][100];
char hash[50][100];
char half_password[50][100];
queue* q;
int finish;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

void* crack(void* thread_id) {
	int tid = (long)thread_id;
	struct crypt_data cdata;
	cdata.initialized = 0;
	long find = 0;
while (1) {
	pthread_mutex_lock(&m);
	long f_finish = finish; 
	pthread_mutex_unlock(&m);
	if (f_finish) break;
	
	long i = (long)queue_pull(q);
	if (i == -1) {
		pthread_mutex_lock(&m);
		finish = 1;
		pthread_mutex_unlock(&m);
		break;
	}
	v1_print_thread_start(tid+1, name[i]);
	char* hash_str = hash[i];
	char* pass = half_password[i];
	char* dot = strstr(pass, ".");
	int pass_len = strlen(pass);
	char* fin = &pass[pass_len-1];
	int num_dots = fin - dot + 1;
	long total_num = pow(26, num_dots);
	char unknowns[num_dots+1];
	memset(unknowns, 'a' , num_dots);
	unknowns[num_dots] = 0;
	long it; 
	int this_find = 0;
	for (it = 0; it < total_num; it++) {
		int n = it;
		int pos = num_dots - 1; // aaaa0
		
		while (n > 0) {
			int m = n % 26;
			unknowns[pos] += m;
			pos--;
			n = (n - m) / 26;
		}
		strcpy(dot, unknowns);
		const char *hashed = crypt_r(pass, "xx", &cdata);
		if (strcmp(hashed, hash_str) == 0) {
			v1_print_thread_result(tid+1, name[i], pass, it+1, getThreadCPUTime(), 0);
			find += 1;
			this_find = 1;
			break;
		}
		memset(unknowns, 'a' , num_dots);
	}
	if (!this_find) v1_print_thread_result(tid+1, name[i], pass, it, getThreadCPUTime(), 1);
}

	return (void*)find;
}



int start(size_t thread_count) {
    // TODO your code here, make sure to use thread_count!
    // Remember to ONLY crack passwords in other threads
	q = queue_create(100);

	ssize_t len = 0;
    size_t n = 0;
	long i = 0;
	char* buffer = NULL;
	while ((len = getline(&buffer, &n, stdin)) != -1) {
		if (buffer[len-1] == '\n') {
			buffer[len-1] = '\0';
		}

		//strcpy(strings[i],buffer);
		char* first_space = strstr(buffer, " ");
		*first_space = '\0';
		strcpy(name[i], buffer); // copy name;
		char* second_space = strstr(first_space+1, " ");
		*second_space = '\0';
		strcpy(hash[i], first_space+1);
		strcpy(half_password[i], second_space+1);
		queue_push(q, (void*)i);

		free(buffer);
		buffer = NULL;
		i++;
	}
	free(buffer);
	queue_push(q, (void*)-1);
	queue_push(q, (void*)-1);
	queue_push(q, (void*)-1);
	queue_push(q, (void*)-1);
	//fprintf(stderr, "jhhhihi\n")
	pthread_t workers[thread_count];
	for (unsigned long j = 0; j < thread_count; j++) {
		pthread_create(&workers[j],NULL, &crack, (void*)j);
	}
	
	
	void* ret;
	long succ = 0;
	for (unsigned j = 0; j < thread_count; j++) {
		pthread_join(workers[j], &ret);
		succ += (long)ret;
	}	
	v1_print_summary(succ, i-succ);

	pthread_mutex_destroy(&m);
    queue_destroy(q);
    return 0; // DO NOT change the return code since AG uses it to check if your
              // program exited normally
}
