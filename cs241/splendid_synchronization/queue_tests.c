/**
 * Splendid Synchronization Lab
 * CS 241 - Fall 2017
 */
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "queue.h"


int main() {
	queue* q = queue_create(10);
	
	for (int i = 0; i < 10; i++) {
		queue_push(q,(void*)0 + i);
	}
	
	
	for (int i = 0; i < 10; i++) {
		printf("%p\n",queue_pull(q));
	}
	

	return 0;
}
