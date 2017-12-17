/**
 * Mini Valgrind Lab
 * CS 241 - Fall 2017
 */

#include <stdio.h>
#include <stdlib.h>

int main() {
    // Your tests here using malloc and free
    char *p1 = malloc(30);
    void *p2 = malloc(40);
    void *p3 = malloc(50);
    
    realloc(p3,60);
   //	realloc(p3,100);
    free(p2);
    free(p1);
    free(p1);
    return 0;
}
