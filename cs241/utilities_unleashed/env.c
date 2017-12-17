/**
 * Lab: Utilities Unleashed
 * CS 241 - Fall 2017
 */
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include "format.h"
#include <string.h>
#include <stdlib.h>

typedef struct 
{
	char* key;
	char* nowval;
} combo;

void free_combo(combo* a, int n) {
	int i = 0;
	for (; i < n; i++) free(a[i].key);
}

void print_combo(combo* a) {
	printf("\nPrinting Combo:\nkey is %s\n",a->key);
	printf("now val is %s\n\n",a->nowval);
}

void check_key_storeKey(char* str,combo* combo_arr,int* key_arr_ptr,char** reftable, int* refnum) {
	char* loc = strstr(str,"%");
	if (loc == NULL) { // if not have ref, store var
		// printf("%s\n",loc);
		char* loceq = strstr(str,"=");
		if (loceq == NULL) print_env_usage();
		// printf("%s\n",loceq);
		*loceq = '\0';
		// combo_arr[*key_arr_ptr].nowval = 
		char* ss = malloc(100);
		memset(ss,0,100);
		strcpy(ss,str);
		*loceq = '=';
		//arr[*key_arr_ptr] = ss;
		combo_arr[*key_arr_ptr].key = ss;
		combo_arr[*key_arr_ptr].nowval = loceq+1; 
		*key_arr_ptr = *key_arr_ptr + 1; // you ji ge key
	} else { // if have ref, make name 
		*loc = '\0';
		char* name_var = malloc(100);
		char alias[100];
		memset(name_var,0,100);
		memset(alias,0,100);
		strcpy(name_var,str);
		strcpy(alias,loc+2);
		*loc = '=';
		*(name_var + strlen(name_var)) = '^';
		*(name_var + strlen(name_var) +1 ) = '\0';
		strcat(name_var,alias); // name+'^'+ref;
		reftable[*refnum] = name_var;
		*refnum = *refnum + 1; 
	}
}

void storekey(char* str,combo* combo_arr,int* key_arr_ptr) {
	char* loceq = strstr(str,"=");
	if (loceq == NULL) print_env_usage();
	*loceq = '\0';
	char* ss = strdup(str);
	*loceq = '=';
	combo_arr[*key_arr_ptr].key = ss;
	combo_arr[*key_arr_ptr].nowval = loceq+1;
	*key_arr_ptr = *key_arr_ptr + 1; // key_arr_ptr is the number of keys
}


int main(int argc, char *argv[]) { 

// find how many values there are for each variable
	if (argc < 4) print_env_usage(); 
	int num_vars_for_each_key = 1;
	// char* reftable[50];
	// int refnum = 0;
	combo combo_arr[50];
	int clr;
	for (clr = 0; clr < 50; clr++) combo_arr[clr].key = combo_arr[clr].nowval = NULL;
	int key_arr_ptr = 0;

	if (argv[1][0] == '-' && argv[1][1] == 'n') {
		// printf("ARE YOU SURE?!\n");

		sscanf(argv[2],"%d",&num_vars_for_each_key);
		if (num_vars_for_each_key < 1) print_env_usage(); 
	}
	int begin_idx_cmd = 0;
	int i = 1;
	if (num_vars_for_each_key > 1) i = 3;

	for (; i < argc; i++) {
		if (strstr(argv[i],"--")) {
			begin_idx_cmd = i+1;
			break;
		}
		storekey(argv[i],combo_arr,&key_arr_ptr);
	}
	// check data

	if (i == argc) print_env_usage(); // error.

	char cmd[50]; 
	memset(cmd,0,50);
	strcpy(cmd,argv[begin_idx_cmd]);
	char** args = argv + begin_idx_cmd;

	pid_t pr;
	int j = 0;
	char vv[100]; memset(vv,0,100);
	int it = 0;
	while (it < num_vars_for_each_key) {
		for (j = 0; j < key_arr_ptr; j++) {
			char* loc = combo_arr[j].nowval;
			char* locdot = strstr(loc,",");
			if (locdot == NULL) {
				strcpy(vv,loc);
			} else {
				*locdot = '\0';
				strcpy(vv,loc);
				*locdot = ',';
				// printf("vv is now here %s\n", vv);
				combo_arr[j].nowval = locdot + 1;
			}
			char* locpercentage = strstr(vv,"%");
			if (locpercentage) {
				char* refenv = getenv(locpercentage+1);
				if (refenv) {
					memset(vv,0,100);
					strcpy(vv,refenv);
					setenv(combo_arr[j].key,vv,1);
				} else {
					memset(vv,0,100);
				}
			} 
			int fenv = setenv(combo_arr[j].key,vv,1);

			if (fenv < 0) print_environment_change_failed();

			
		//	printf("%s\n",vv);
			memset(vv,0,100);
		}
		pr = fork();
		if (pr == 0) break;	
		waitpid(pr,NULL,0);
		for (int jj = 0; jj < key_arr_ptr; jj++) unsetenv(combo_arr[jj].key);
		it++;
	}
	if (pr < 0) print_fork_failed();
	if (pr == 0) {	
//		printf("%s\n",getenv("ABC"));
		int fexe = execvp(cmd,args);
		if (fexe < 0) print_exec_failed(); 
	}

	free_combo(combo_arr,key_arr_ptr);

	return 0; 
}
