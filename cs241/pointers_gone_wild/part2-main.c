/**
 * Lab: Pointers Gone Wild
 * CS 241 - Fall 2017
 */

#include "part2-functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * (Edit this function to print out the "Illinois" lines in
 * part2-functions.c in order.)
 */
int main() {
    // your code here
	first_step(81);
	
	int a = 132;
	second_step(&a);
	
	int** b;
	b = malloc(sizeof(int*));
	*b = malloc(sizeof(int));
	**b = 8942;
	double_step(b);
	free(*b);
	free(b);
	
	
	int kk = 15;
	
	char chr[6];
	chr[5] = (char)15; 
	strange_step((char*)&kk - 5);
	
	chr[3] = '\0';
	empty_step(chr);
	
	char* ss = chr;
	chr[3] = 'u';
	two_step(ss,chr);
	
	three_step(chr,chr+2,chr+4);
	
	char x,y,z;
	x = '\0';
	y = 8;
	z = 16;
	step_step_step(&x-1,&y-2,&z-3);
	
	char aaa = 'A';
	it_may_be_odd(&aaa,65);
	
	char stt[] = "sd,CS241,    ";
	tok_step(stt);
	
	int blue = 513;
	
	the_end(&blue,&blue); 
    
    return 0;
}
