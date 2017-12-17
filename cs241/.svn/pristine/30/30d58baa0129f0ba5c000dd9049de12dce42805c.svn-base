/**
 * Splendid Synchronization Lab
 * CS 241 - Fall 2017
 */
#include "queue.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * This queue is implemented with a linked list of queue_nodes.
 */
typedef struct queue_node {
    void *data;
    struct queue_node *next;
} queue_node;

struct queue {
    /* queue_node pointers to the head and tail of the queue */
    queue_node *head, *tail;

    /* The number of elements in the queue */
    ssize_t size;

    /**
     * The maximum number of elements the queue can hold.
     * max_size is non-positive if the queue does not have a max size.
     */
    ssize_t max_size;

    /* Mutex and Condition Variable for thread-safety */
    pthread_cond_t cv;
    pthread_mutex_t m;
};

queue *queue_create(ssize_t max_size) {
    /* Your code here */
    queue* newq = malloc(sizeof(struct queue));
    newq->head = newq->tail = NULL;
    newq->size = 0;
    newq->max_size = max_size > 0 ? max_size : (ssize_t)((size_t)-1 >> 1);
    pthread_mutex_init(&(newq->m), NULL);
	pthread_cond_init(&(newq->cv), NULL);
	
	return newq;
	
}


void queue_destroy(queue *this) {
    /* Your code here */
    queue_node* it = this->head;
    while (it != NULL) {
		queue_node* tt = it;
		it = it->next;
		free(tt);
    }
    pthread_mutex_destroy(&this->m);
	pthread_cond_destroy(&this->cv);
	free(this);
}

void queue_push(queue *this, void *data) {
    /* Your code here */
    pthread_mutex_lock(&this->m);
    queue_node* t = malloc(sizeof(queue_node));
    t->next = NULL; t->data = data;
    
	while (this->max_size > 0 && this->size == this->max_size) {
		pthread_cond_wait(&this->cv,&this->m);
	}
	
	if (this->size == 0) {
		this->head = this->tail = t;
	} else {
		this->tail->next = t;
		this->tail = t;
	}
	
	this->size++;
	if (this->size == 1) {
		pthread_cond_broadcast(&this->cv);
	}
	    
    pthread_mutex_unlock(&this->m);
}

void *queue_pull(queue *this) {
    /* Your code here */
	pthread_mutex_lock(&this->m);   
	while (this->size == 0) {
		pthread_cond_wait(&this->cv, &this->m);
	}
	void* ret = this->head->data;
	queue_node* temp = this->head;
	this->head = this->head->next;
	temp->next = NULL;
	free(temp);
	if (this->size == 1) this->tail = NULL;
	this->size--;
	if (this->size == this->max_size - 1) {
        pthread_cond_broadcast(&(this->cv));
    }
	
	pthread_mutex_unlock(&this->m);
    return ret;
}
