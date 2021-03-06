/**
 * Teaching Threads
 * CS 241 - Fall 2017
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "reduce.h"
#include "reducers.h"

/* You might need a struct for each task ... */
typedef struct _par {
	int* list;
	int len;
	reducer reduce_func;
	int base_case;
	int it;
	int* head;
} par;

/* You should create a start routine for your threads. */
void* start_routine(void* arg) {
	par* p = arg;
	p->head[p->it] = reduce(p->list,p->len,p->reduce_func,p->base_case);
	return NULL;
}


int par_reduce(int *list, size_t list_len, reducer reduce_func, int base_case,
               size_t num_threads) {
    /* Your implementation goes here */
	if (num_threads > list_len) num_threads = list_len;
	
	
	int rem = list_len % num_threads;
	int div = list_len / num_threads;
	size_t i = 0;
	par p_arr[100];
	int new_list[num_threads];
	
	
		
	pthread_t threads[100];
	for (i = 0; i < num_threads; i++) {
		p_arr[i].reduce_func = reduce_func;
		p_arr[i].base_case = base_case;
		p_arr[i].it = i;
		p_arr[i].head = new_list;
		if (i != num_threads - 1) {
			p_arr[i].list = list + i * div;
			p_arr[i].len = div;
		} else {
			p_arr[i].list = list + i * div;
			p_arr[i].len = div + rem;
		}
		pthread_create(&threads[i],NULL,&start_routine,&p_arr[i]);
	}
	for (i = 0; i < num_threads; i++) pthread_join(threads[i],NULL);
    
    int retval = reduce(new_list,num_threads,reduce_func,base_case);
    
    return retval;
}
