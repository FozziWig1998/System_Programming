/**
 * Machine Problem: Shell
 * CS 241 - Fall 2017
 */
#include <string.h>
#include <stdlib.h>
#include "format.h"
#include "shell.h"
#include "vector.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>



typedef struct process {
    char *command;
    char *status;
    pid_t pid;
} process;

int kk;
pid_t child[10];
int child_it;
int fd;

int buffer_operator(char* buffer, char two_buffers[2][100] ) {
    char* loc; char* locc = NULL; 
    if ((loc = strstr(buffer,"&&"))) {
        *loc = '\0';
        locc = loc-1;
        while (*locc == ' ') locc--;
        char t = locc[1];
        locc[1] = '\0';
        // if (*(loc-1) == ' ') loc[-1] = '\0'; 
        strcpy(two_buffers[0],buffer);
        locc[1] = t;
        *loc = '&';
        loc = loc + 2; 
        while(*loc == ' ') loc++;
        strcpy(two_buffers[1],loc);
        return 1;
    } else if ((loc = strstr(buffer,"||"))) {
        *loc = '\0';
        locc = loc-1;
        while (*locc == ' ') locc--;
        char t = locc[1];
        locc[1] = '\0';
        // if (*(loc-1) == ' ') loc[-1] = '\0'; 
        strcpy(two_buffers[0],buffer);
        *loc = '|';
        locc[1] = t;
        loc = loc + 2; 
        while(*loc == ' ') loc++;
        strcpy(two_buffers[1],loc);
        return 2;
    } else if ((loc = strstr(buffer,";"))) {
        *loc = '\0';
        locc = loc-1;
        while (*locc == ' ') locc--;
        char t = locc[1];
        locc[1] = '\0';
        // if (*(loc-1) == ' ') loc[-1] = '\0'; 
        strcpy(two_buffers[0],buffer);
        *loc = ';';
        locc[1] = t;
        loc = loc + 2; 
        while(*loc == ' ') loc++;
        strcpy(two_buffers[1],loc);
        return 3;
    } else {
        return 0;
    }
}




int run(char* buffer, vector* history_vector, int f_print_prompt, int f_print_cmd) { // if ret = -1, exit abnormally
	if (kk == 1) return 0;
    int haha = 0;
    if (f_print_prompt) {
	   char cwd[5000]; memset(cwd,0,5000);
	   pid_t nowpid = getpid();
	   getcwd(cwd,sizeof(cwd));
	   print_prompt(cwd,nowpid);
    }  
	if (f_print_cmd) print_command(buffer);
	size_t vec_size = vector_size(history_vector);
	char* ptr;
  	if ((ptr = strstr(buffer,"cd "))) {
    	char path[100]; memset(path,0,100);
    	strcpy(path,ptr+3);
        
    	//fprintf(stderr,"path is %s\n",path);
    	if (( haha = chdir(path)) < 0) {
    		print_no_directory(path);
    	}
    	vector_push_back(history_vector,buffer);
        if (haha < 0) return -1;
    } else if (strcmp(buffer,"!history") == 0) {
    	for (size_t i = 0; i < vec_size; i++) {
    		print_history_line(i,(const char *)vector_get(history_vector,i));
    	}
    } else if ((ptr = strstr(buffer,"#"))) { 
    	int num;
    	// fprintf(stderr, "%s\n", buffer);
    	sscanf(ptr+1,"%d",&num);
    	// fprintf(stderr,"num is %d\n",num);
    	char command[100]; memset(command,0,100);
    	
    	if (num >= (int)vec_size) { 
    		print_invalid_index();
    	} else {
    		strcpy(command,(const char *)vector_get(history_vector,num));
    		run(command,history_vector,0,1);
    	} 
    } else if (*buffer == '!') {
    	char prefix[100]; memset(prefix,0,100);
    	strcpy(prefix,buffer+1);
    	unsigned long prefix_len = strlen(prefix);
    	if (*prefix == 0) {
    		run((char *)vector_get(history_vector,vec_size-1),history_vector,0,1);
    	} else {
    		int i;
    		for (i = vec_size - 1; i >= 0; i--) {
    			char* command = (char *)vector_get(history_vector,i);
    			if (strlen(command) < prefix_len) continue;
    			char t = command[prefix_len]; 
    			command[prefix_len] = '\0';
    			if (strcmp(command,prefix) == 0) {
    				command[prefix_len] = t;
    				run(command,history_vector,0,1);
    				break;
    			} else {
    				command[prefix_len] = t;
    			}
    		}
    		if (i < 0) print_no_history_match();
    	} 
    } else if (strcmp(buffer,"exit") == 0) {
    	// vector_clear(history_vector);
    	vector_destroy(history_vector);
    	exit(0);
    } else {
    	// run External Commands;
   		vector_push_back(history_vector,buffer);
    	//char** args = strsplit(buffer,&space,&numtokens);
        pid_t pr = fork();
   		int status;
   		if (pr < 0) print_fork_failed();
   		if (pr > 0) {
   		 // I am father.
   		   if (setpgid(pr, pr) == -1) {
   			 print_setpgid_failed();
   			 exit(1);
  			}
            child[child_it] = pr;
            child_it++;
   			waitpid(pr,&status,0);
            haha = WEXITSTATUS(status);
            // fprintf(stderr,"haha is %d\n",haha );
            // if (haha == 0) return 0;
            // else return -1;
            if (haha != 0) return -1;
            else return 0;
   		} else {
            char* args[1000];
            char* tempbuf = strtok(buffer," ");
            int i = 0;
            while (tempbuf != NULL) {
                args[i++] = tempbuf;
                tempbuf = strtok(NULL," ");
            }
            args[i] = NULL;

   			print_command_executed(getpid());
   			int zt = execvp(*args,args);
   			if (zt < 0) print_exec_failed(buffer);
   			// kill(getpid(), SIGINT);
   			// free_args(args);
   			kk = 1;

            // char** ptr = args;
            // while (*ptr) {
            //     free(*ptr);
            //     ptr++;
            // }
            exit(9);
   		}
        //free_args(args);
        /*while (*args) {
            free(*args);
            args++;
        }
        free(args);
        */
    }
    return 0;
}

void write_his_to_file(vector* history_vector) {
	
	size_t vec_size = vector_size(history_vector);
	for (size_t i = 0; i < vec_size; i++) {
		dprintf(fd,"%s\n",(char *)vector_get(history_vector,i));
	}
}


void filemode(char* filename, int flaghis, vector* history_vector) {
	FILE* input = fopen(filename,"r");
	if (!input) { 
		print_script_file_error();
		return;
	}	
	ssize_t len = 0;
    size_t n = 0;
  //  int i = 0;
    char* buffer = NULL;
    while (( len = getline(&buffer,&n,input)) != -1) {
    	if (buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        char two_buffers[2][100];
        int option = buffer_operator(buffer,two_buffers);
        if (option == 0) {
            run(buffer,history_vector,1,1);
        } else if (option == 3) {
            run(two_buffers[0],history_vector,1,1);
            run(two_buffers[1],history_vector,1,1);
        } else if (option == 1) {
            if (run(two_buffers[0],history_vector,1,1) == 0)
                run(two_buffers[1],history_vector,1,1);
        } else if (option == 2) {
            if (run(two_buffers[0],history_vector,1,1) == 0) {
                continue;
            } else {
                run(two_buffers[1],history_vector,1,1);
            }
        }

        // run(buffer,history_vector,1,1);
    	free(buffer);
    	buffer = NULL;
    }
    if (buffer) free(buffer);
    if (flaghis) {
    	write_his_to_file(history_vector);
    }
    vector_destroy(history_vector);
    fclose(input);
	exit(0);    
}

void signalhandlerkill() {
    for (int i = 0; i < 10; i++) {
        if (child[i] > 0) {  
            kill(child[i],SIGKILL);
        }   
    }
}
void signalhandlerEOF() {
    exit(0);
}

int shell(int argc, char *argv[]) {
    
    kk = 0;
    child_it = 0;
    for (int i = 0; i < 10; i++) child[i] = 0;
    signal(SIGINT,signalhandlerkill);
    // signal(EOF,signalhandlerEOF);
    // TODO: This is the entry point for your shell.
    int flaghis = 0, flagfile = 0;
    if (argc == 2 || argc == 4 || argc > 5) print_usage();
    vector* history_vector = string_vector_create();
//	vector_push_back(history_vector,firstline);
    char command_file[100]; memset(command_file,0,100);
    char hisfile[100]; memset(hisfile,0,100);
    while (*argv) {
    	if (strcmp(*argv,"-h") == 0) {
    		flaghis = 1;
    		strcpy(hisfile,argv[1]);
    		fd = open(hisfile,O_CREAT | O_TRUNC | O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO);
    	}
    	if (strcmp(*argv,"-f") == 0) {
    		flagfile = 1;
    		strcpy(command_file,argv[1]);
    		
    	}
    	argv++;
    }

    if (flagfile) filemode(command_file,flaghis,history_vector);

    while (1) {
    	/*pid_t nowpid = getpid();
    	getcwd(cwd,sizeof(cwd));
    	print_prompt(cwd,nowpid);
    	*/
        char cwd[5000]; memset(cwd,0,5000);
        pid_t nowpid = getpid();
        getcwd(cwd,sizeof(cwd));
        print_prompt(cwd,nowpid);
        
    	char buffer[500]; memset(buffer,0,500);
        if (feof(stdin)) {
            if (flaghis) write_his_to_file(history_vector); 
            return 0;
        }    
    	fgets(buffer,100,stdin);
    	// fprintf(stderr, "%s len = %lu\n",buffer,strlen(buffer) );
        buffer[strlen(buffer)-1] = 0;
        if (buffer[0] == EOF) exit(0);
        char two_buffers[2][100];
        // fprintf(stderr, "%s len = %lu\n",buffer,strlen(buffer) );
        int option = buffer_operator(buffer,two_buffers);
    	if (option == 0) {
            run(buffer,history_vector,0,0);
        } else if (option == 3) {
            run(two_buffers[0],history_vector,0,0);
            run(two_buffers[1],history_vector,0,0);
        } else if (option == 1) {
            if (run(two_buffers[0],history_vector,0,0) == 0)
                run(two_buffers[1],history_vector,0,0);
        } else if (option == 2) {
            if (run(two_buffers[0],history_vector,0,0) == 0) {
                continue;
            } else {
                run(two_buffers[1],history_vector,0,0);
            }
        }
    	// break;
    }
	return 0;
}
