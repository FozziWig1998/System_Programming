/**
 * MP: Extreme Edge Cases
 * CS 241 - Fall 2017
 */
#include "camelCaser.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

char* makecamel(char* buffer) {
	int first = 1;
	char* output = (char*)malloc(sizeof(char)*(strlen(buffer)+1));
	char* ptrout = output;
	int nextup = 0;
	while (*buffer && isspace(*buffer)) {
		buffer++;
	}
	if (!isalpha(*buffer)) first = 0;
	//printf("now stris:%s\n",buffer);
	while (*buffer) {
		if (first == 1 && isalpha(*buffer)) {
			if (*buffer < 'a') 
				*ptrout = *buffer + 32;
			else 
				*ptrout = *buffer;	
			first = 0; 
		} else if (isspace(*buffer)) {
			nextup = 1;
			ptrout--;
		} else if (isalpha(*buffer) && nextup) {
			if (*buffer > 96) 
				*ptrout = *buffer - 32;
			else 
				*ptrout = *buffer;	
			nextup = 0;
		} else if (isalpha(*buffer) && nextup == 0) {
			if (*buffer < 'a')
				*ptrout = *buffer + 32;
			else 
				*ptrout = *buffer;	
		} else {
			*ptrout = *buffer;
		}
		
		buffer++; 
		ptrout++;
	}
	*ptrout = '\0';
//	printf("%s******\n",output);
	return output;
}



char **camel_caser(const char *input_str) {
    if (input_str == NULL) return NULL;
    int counter = 0;
    int i = 0;
	for (i = 0; i < (int)strlen(input_str); i++) {
		if (ispunct(input_str[i])) counter++;
	}
	char** retval = (char**)malloc(sizeof(char*)*(counter+1));
	char** retptr = retval;    
	char buffer_input[100000];
    char* ptrin = buffer_input;
	while (*input_str) {
		if (ispunct(*input_str)) {
			*ptrin = '\0';
			//printf("input_str:%s\n",buffer_input);
			char* output_str = makecamel(buffer_input);
			*retptr = output_str;
			retptr++;
			//printf("%s\n",output_str);
			ptrin = buffer_input - 1;
			
		} else {
			*ptrin = *input_str;
		}
		ptrin++;
		input_str++;
	}
	*retptr = NULL;
	return retval;
}

void destroy(char **result) {
    if (result == 0) return;
    char** ptr = result;
    while (*ptr) {
    	free(*ptr);
    	ptr++;
    }
    free(*ptr);
    free(result);
}
