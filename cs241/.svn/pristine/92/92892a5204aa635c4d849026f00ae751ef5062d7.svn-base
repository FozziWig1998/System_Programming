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
#include <ctype.h>



typedef struct process {
    char *command;
    char *status;
    pid_t pid;
} process;

int kk;
pid_t child[50];
process all_processes[50];
int child_it;
char* argv0;
int fd;
int flaghis;
int operator_used;
int flagfile;

void pre_wash_dollar(char* buffer) {
	char* locc = buffer;
	char temp[100]; memset(temp,0,100);
	char* ptr = buffer;
	while ((locc = strstr(locc,"\\$"))) {
		*locc = 0;
		strcat(temp,ptr);
		strcat(temp,"\t");
		locc += 2;
		ptr = locc;
	}
	strcat(temp,ptr);
	//printf("%s\n",temp);
	if (! flagfile) { 
	// cmd_mode, buffer is on stk;
		//memset(buffer,0,100);
		strcpy(buffer,temp);
	} else { 
	//	file_mode, buffer on heap;
		char* new_buffer = strdup(temp);
		free(buffer);
		buffer = new_buffer; 	
	}
}

void wash_restore(char* buffer) {
	while (*buffer) {
		if (*buffer == '\t') *buffer = '$';
		buffer++;
	}
}

void ref_replace(char* buffer) {
	char ref[100]; memset(ref,0,100);
	char res[100]; memset(res,0,100); //result buf
	char* find_ptr, *ptr, *ptrbef;
	ptr = ptrbef = buffer;
    if (strchr(ptr,'$') == NULL) return;
	while ((ptr = strchr(ptr,'$'))) {
		*ptr = '\0';
		strcat(res,ptrbef);
		*ptr = '$';
		
		find_ptr = ptr + 1;
		
		while (*find_ptr) {
			if (!isalpha(*find_ptr) && !isdigit(*find_ptr) && !(*find_ptr == '_')) {
				break;
			}	
			find_ptr++;
		}

		char tp = *find_ptr;
		*find_ptr = '\0';
		strcpy(ref,ptr + 1); //get ref val
		*find_ptr = tp;
		if (getenv(ref))
			strcat(res,getenv(ref));

		ptrbef = find_ptr;
		ptr = find_ptr + 1;
				
	}
	strcat(res,find_ptr);
	//printf("%s\n",res);
	if (flagfile) {
		char* new_buffer = strdup(res);
		free(buffer);
		buffer = new_buffer; 	
	} else {
		memset(buffer,0,100);
		strcpy(buffer,res);
	}
}



void write_his_to_file( vector* history_vector) {
	size_t vec_size = vector_size(history_vector);
	for (size_t i = 0; i < vec_size; i++) {
		dprintf(fd,"%s\n",(char *)vector_get(history_vector,i));
	}
}

int buffer_operator(char* buffer, char two_buffers[2][100], vector* history_vector ) {
    char* loc; char* locc = NULL; 
    if ((loc = strstr(buffer,"&&"))) {
        vector_push_back(history_vector,buffer);
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
        vector_push_back(history_vector,buffer);
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
    } else if ((loc = strstr(buffer,";")) && loc[-1] != '\\') {
        vector_push_back(history_vector,buffer);
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
  		// yi jing cun ru his file, kai shi xia gao
  		pre_wash_dollar(buffer); //  escape $-> \t
  		ref_replace(buffer);
  		
  		char* bufit = buffer;
  		while (*bufit) {
  			if (*bufit == '\t') *bufit = '$';
  			bufit++;
  		}
  		
  		
    	char path[100]; memset(path,0,100);
    	strcpy(path,ptr+3);
        
    	//fprintf(stderr,"path is %s\n",path);
    	if (( haha = chdir(path)) < 0) {
    		print_no_directory(path);
    	}
    	if (! operator_used) vector_push_back(history_vector,buffer);
        if (haha < 0) return -1;
    } else if (strcmp(buffer,"ps") == 0) {
    	print_process_info("Running",getpid(),argv0);
    	for (int i = 0; i < child_it; i++) {
    		if (strcmp(all_processes[i].status,"NO_READ") == 0) continue;
    		char cmd[100]; memset(cmd,0,100);
    		strcpy(cmd,all_processes[i].command);
			int st, st2;
    		int child_status = waitpid(all_processes[i].pid,&st,WNOHANG);
    		char* st_print;
    		waitpid(all_processes[i].pid,&st2,WUNTRACED | WNOHANG);
    		if (child_status == all_processes[i].pid || child_status == -1) {
    			all_processes[i].status = "NO_READ"; 
    			continue;
    		
    		} else if ((strcmp(all_processes[i].status,"Stopped") == 0) ) {
    				st_print = "Stopped";
    				print_process_info(st_print,all_processes[i].pid,cmd);
    		} else if (child_status == 0) {
    			st_print = "Running";
    			print_process_info(st_print,all_processes[i].pid,cmd);
    		}	
    	}
    	
    	return 0;
    } else if ((ptr = strstr(buffer,"kill")) || (ptr = strstr(buffer,"stop")) || (ptr = strstr(buffer,"cont"))) {
    	if (! operator_used) vector_push_back(history_vector,buffer);
		int pid_to_kill;
		char* tp_ptr = ptr;
		tp_ptr += 3;
		while (*tp_ptr) {
			if (*tp_ptr >= '0' && *tp_ptr <= '9') break;
			tp_ptr++;
		}
		if (! *tp_ptr) {
			print_invalid_command(buffer);
			return -1;
		}	
		sscanf(tp_ptr,"%d",&pid_to_kill);
        int st;
        int res = waitpid((pid_t)pid_to_kill,&st,WNOHANG);
        if (res == -1 || res == pid_to_kill) {
            print_no_process_found(pid_to_kill);
            return -1;
        }
		//fprintf(stderr, "buffer is:%s,pid_to_kill is:%d\n",buffer,pid_to_kill);
        if (strstr(buffer,"kill")) {
            kill(pid_to_kill,SIGTERM);
            
            for (int it_kill = 0; it_kill < child_it; it_kill++) {
                if (all_processes[it_kill].pid == pid_to_kill) {
                    all_processes[it_kill].status = "NO_READ";
                    print_killed_process(pid_to_kill,all_processes[it_kill].command);
                    return 0;
                }   
            }
        } else if (strstr(buffer,"stop")) {
            kill(pid_to_kill,SIGTSTP);
            for (int it_kill = 0; it_kill < child_it; it_kill++) {
                if (all_processes[it_kill].pid == pid_to_kill) {
                    all_processes[it_kill].status = "Stopped";
                    print_stopped_process(pid_to_kill,all_processes[it_kill].command);
                    return 0;
                }   
            }
        } else if (strstr(buffer,"cont")) {
            kill(pid_to_kill,SIGCONT);
            for (int it_kill = 0; it_kill < child_it; it_kill++) {
                if (all_processes[it_kill].pid == pid_to_kill) {
                    if (strcmp(all_processes[it_kill].status,"Stopped") == 0) 
                        all_processes[it_kill].status = "Running";
                    return 0;
                }   
            }            
        }
		
    } else if ((ptr = strstr(buffer,"export"))) {
		if (!operator_used) vector_push_back(history_vector,buffer);
		char var[100]; memset(var,0,100);
		//char ref[100]; memset(var,0,100);
		ptr += 6;
		while (*ptr == ' ') ptr++;
		char* ptr_eq = strstr(ptr,"="); 
		*ptr_eq = '\0';
		strcpy(var,ptr); // var is the name of env_var
		*ptr_eq = '=';
		//char* ptr = ptr_eq + 1;
		//clear
		char settt[100]; memset(settt,0,100);
		strcpy(settt,ptr_eq+1);
		pre_wash_dollar(settt);
		ref_replace(settt);
		wash_restore(settt);
		//fprintf(stderr,"SETVAL= %s\n",settt);
		//fprintf(stderr,"ENVAR= %s\n",var);
		setenv(var,settt,1);
		
    	
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
    	if (flaghis) write_his_to_file(history_vector);
    	vector_destroy(history_vector);
    	for (int i = 0; i < child_it; i++) {
    		int pid = all_processes[i].pid;
    		char* status = all_processes[i].status;
    		if (*status == 'R' || *status == 'S') {
    			kill(pid,SIGTERM);
    		}
    		free(all_processes[i].command);
    	}
    	exit(0);
    } else {
    	if (!operator_used) vector_push_back(history_vector,buffer);
    	//f_background = strstr(buffer,"&");
    	//f_background = f_background && 
		char* f_background = NULL;
    	int len_buf = strlen(buffer);
    	for (int i = len_buf - 1 ; i >= 0; i--) {
    		if (buffer[i] == '&') {
    			f_background = buffer + i;
    			if (buffer[i-1] == '\\') f_background = NULL;
    			else *f_background = '\0';
    			break;
    		}  
    	}
    	
    	
    	// run External Commands;
		//fprintf(stderr,"external !!!\n");

		
   		pid_t pr = fork();
   		int status;
   		if (pr < 0) print_fork_failed();
   		if (pr > 0) {
   		 // I am father.
			  if (setpgid(pr, pr) == -1) {
    			print_setpgid_failed();
    			exit(1);
  			}
   		 
            //child[child_it] = pr;
            //child_it++;
   			if (! f_background) {
   				waitpid(pr,&status,0);
        	    haha = WEXITSTATUS(status);
       	     // fprintf(stderr,"haha is %d\n",haha );
       	     // if (haha == 0) return 0;
       	     // else return -1;
        	    if (haha != 0) return -1;
        	    else return 0;
            } else {
            	process newp;
            	newp.command = strdup(buffer);
            	newp.status = "Running";
            	newp.pid = pr;
            	all_processes[child_it] = newp;
            	child_it++;
            	waitpid(pr,&status,WNOHANG);
            	return 0;
            }
   		} else {
   			// I am child;
   			pre_wash_dollar(buffer);
			ref_replace(buffer);
			wash_restore(buffer);

			char* loc_escap;
			char temp[1000]; memset(temp,0,1000);
		// wash my buffer!!!
			while ((loc_escap = strchr(buffer,'\\'))) {
				*loc_escap = '\0';
				strcat(temp,buffer);
				// abc\&\|ssskk\;
				buffer = loc_escap + 1;
			}
			if (*temp) {
				strcat(temp,buffer);
				buffer = temp;		
				//fprintf(stderr,"WASH BUFFER %s\n",(char*)temp);
			}
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
        
        /*while (*args) {
            free(*args);
            args++;
        }
        */
    }
    return 0;
}


void filemode(char* filename, int flaghis, vector* history_vector ) {
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
        int option = buffer_operator(buffer,two_buffers, history_vector);
        if (option) operator_used = 1;
        
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
        operator_used = 0;

        // run(buffer,history_vector,1,1);
    	free(buffer);
    	buffer = NULL;
    }
    if (buffer) free(buffer);
    if (flaghis) {
    	write_his_to_file(history_vector);
    }
    vector_destroy(history_vector);
    for (int i = 0; i < child_it; i++) {
    	int pid = all_processes[i].pid;
    	char* status = all_processes[i].status;
    	if (*status == 'R' || *status == 'S') {
    		kill(pid,SIGTERM);
    	}
    	free(all_processes[i].command);
    }
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

/*void signalhandlerEOF() {
    exit(0);
}


typedef struct process {
    char *command;
    char *status;
    pid_t pid;
} process;
*/

int shell(int argc, char *argv[]) {
    kk = 0;
    child_it = 0;
	argv0 = argv[0];
    
    for (int i = 0; i < 10; i++) child[i] = 0;
    signal(SIGINT,signalhandlerkill);
    // signal(EOF,signalhandlerEOF);
    // TODO: This is the entry point for your shell.
    flaghis = 0;
    flagfile = 0;
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
            if (flaghis) write_his_to_file( history_vector); 
            vector_destroy(history_vector);
            for (int i = 0; i < child_it; i++) {
    			int pid = all_processes[i].pid;
    			char* status = all_processes[i].status;
    			if (*status == 'R' || *status == 'S') {
    			kill(pid,SIGTERM);
    			}
    			free(all_processes[i].command);
    		}
    		//fprintf(stderr,"exit~~~\n");
            return 0;
        }    
    	fgets(buffer,100,stdin);
    	// fprintf(stderr, "%s len = %lu\n",buffer,strlen(buffer) );
        buffer[strlen(buffer)-1] = 0;
        if (buffer[0] == EOF) exit(0);
        char two_buffers[2][100];
        // fprintf(stderr, "%s len = %lu\n",buffer,strlen(buffer) );
        int option = buffer_operator(buffer,two_buffers, history_vector);
        if (option)	operator_used = 1;
    	
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
        operator_used = 0;
    	// break;
    }
	return 0;
}