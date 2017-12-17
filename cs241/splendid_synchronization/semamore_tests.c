/**
 * Splendid Synchronization Lab
 * CS 241 - Fall 2017
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "semamore.h"

int main() {
    printf("Please write tests in semamore_tests.c\n");
    
    Semamore *s = malloc(sizeof(Semamore));
    semm_init(s, 5, 10);
    
    for (int i = 0; i < 6; i++) {
    	fprintf(stderr,"val in semamore is %d\n", s->value);
    	semm_wait(s);
    }	
    
    
    return 0;
}
