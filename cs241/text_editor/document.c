/**
 * Machine Problem: Text Editor
 * CS 241 - Fall 2017
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "document.h"
#include "vector.h"


struct document {
    vector *vector;
};

document *document_create() {
    document *this = (document *)malloc(sizeof(document));
    assert(this);
    this->vector = vector_create(string_copy_constructor, string_destructor,
                                 string_default_constructor);
    return this;
}
void document_write_to_file(document *this, const char *path_to_file) {
    assert(this);
    assert(path_to_file);
    // see the comment in the header file for a description of how to do this!
    // TODO: your code here!
    size_t i = 1;
    int fd = open(path_to_file,O_CREAT | O_TRUNC | O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO);
    
    for (i = 1; i <= document_size(this); i++) {
    	dprintf(fd,"%s\n",document_get_line(this,i));
    }
}



document *document_create_from_file(const char *path_to_file) {
    assert(path_to_file);
    // this function will read a file which is created by document_write_to_file
    // TODO: your code here!
    document* retval = document_create();
    FILE* input = fopen(path_to_file,"r");
    if (input == NULL) {
   // 	perror("Cannot open?!");
    	return retval;
    }
    char* buffer = NULL;
    size_t n;
    ssize_t len = 0;
    // int k = 0;
	while ((len = getline(&buffer,&n,input)) != -1 ) { 
		char* newline = malloc(len * sizeof(char) + 10);
        memset(newline,0,len * sizeof(char) + 10);
		// printf("LET ME TEST CHAR %d\n", buffer[len-1] );

        if (buffer[len - 1] == '\n') {
           // printf("buffer is %s\n", buffer);
            // printf("len=%zd\n",len );
            // printf("strlen=%lu\n",strlen(buffer) );
             // printf("buffer[11] is %d\n",buffer[12]);
            // buffer[len - 1] = 0;
            buffer[len - 1] = '\0';
            // printf("strlen buf=%lu\n",strlen(buffer));
            // printf("char last first %d\n",buffer[len - 1] );
        }
        
        // printf("wo cun jin qu de %s\n",buffer);
        // printf("char last %d\n",buffer[strlen(buffer)-1] );
        // printf("%lu\n",strlen(buffer));

		strcpy(newline,buffer);
		free(buffer);
		buffer = NULL;
		// document_insert_line(retval,++k,newline);
       vector_push_back(retval->vector,newline);
       free(newline);
       newline = NULL;
	}
	free(buffer);
    // free(newline);
	buffer = NULL;
    fclose(input);
	return retval;
}






/*document *document_create_from_file(const char *path_to_file) {
    assert(path_to_file);
    // this function will read a file which is created by document_write_to_file
    // TODO: your code here!
    return NULL;
}
*/

void document_destroy(document *this) {
    assert(this);
    vector_destroy(this->vector);
    free(this);
}

size_t document_size(document *this) {
    assert(this);
    return vector_size(this->vector);
}

void document_set_line(document *this, size_t line_number, const char *str) {
    assert(this);
    assert(str);
    size_t index = line_number - 1;
    vector_set(this->vector, index, (void *)str);
}

const char *document_get_line(document *this, size_t line_number) {
    assert(this);
    assert(line_number > 0);
    size_t index = line_number - 1;
    return (const char *)vector_get(this->vector, index);
}

/*void document_insert_line(document *this, size_t line_number, const char *str) {
    assert(this);
    assert(str);
    // TODO: your code here!
    // How are you going to handle the case when the user wants to
    // insert a line past the end of the document?
}*/

void document_insert_line(document *this, size_t line_number, const char *str) {
    assert(this);
    assert(str);
    // TODO: your code here!
    // How are you going to handle the case when the user wants to
    // insert a line past the end of the document?
    size_t size = document_size(this);
    if (line_number > size) {
    	size_t i;
    	for (i = size; i < line_number-1; i++) {
            char* empstr = malloc(1);
            *empstr = '\0';
    		vector_push_back(this->vector,empstr);
            free(empstr);
            empstr = NULL;
    	}

        char* ss = malloc(strlen(str)+1);
    	strcpy(ss,str);
    	vector_push_back(this->vector,ss);
    	free(ss);
        ss = NULL;


    } else {
    	char* ss = malloc(strlen(str)+1);
    	strcpy(ss,str);
    	vector_insert(this->vector,line_number-1,ss);
    	free(ss);
        ss = NULL;
    }  
    
}



void document_delete_line(document *this, size_t line_number) {
    assert(this);
    assert(line_number > 0);
    size_t index = line_number - 1;
    vector_erase(this->vector, index);
}
