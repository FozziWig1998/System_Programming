/**
 * Machine Problem: Text Editor
 * CS 241 - Fall 2017
 */
#include "document.h"
#include "editor.h"
#include "format.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *get_filename(int argc, char *argv[]) {
    // TODO implement get_filename
    // take a look at editor_main.c to see what this is used for
    return argv[argc-1];
}

document *handle_create_document(const char *path_to_file) {
    // TODO create the document
    return document_create_from_file(path_to_file);
    //pass
}

void handle_cleanup(document *document) {
    // TODO destroy the document
	document_destroy(document);
    //pass
}

void handle_display_command(document *document, size_t start_line,
                            ssize_t max_lines, size_t start_col_index,
                            ssize_t max_cols) {
    // TODO implement handle_display_command
    // THIS FN PASS.

	if (document == NULL || document_size(document) == 0){
		print_document_empty_error();
		return;
	}
    size_t size = document_size(document);
	if (max_lines == -1) {
		size_t i = 1;
		for (;i <= size; i++) {
			print_line(document,i,0,-1);	
		}
		return;
	}
    size_t now = start_line;
    int cc = 0;
    for (;now <= size && cc++ < max_lines; now++) {
        print_line(document,now,start_col_index,max_cols);
    }
	
}

void handle_insert_command(document *document, location loc, const char *line) {
    if (loc.line_no > document_size(document)) {
        document_insert_line(document,loc.line_no,line);
    } else {
        char buf[1000];
        memset(buf,0,sizeof(buf));
        strcpy(buf,document_get_line(document,loc.line_no));
        char* lochar = buf + loc.idx;
        char houban[1000];
        memset(houban,0,sizeof(houban));
        strcpy(houban,lochar);
        buf[loc.idx] = '\0';
        strcat(buf,line);
        strcat(buf,houban);
        document_set_line(document,loc.line_no,buf);
        // free(buf);
        // buf = NULL;
    }
}

void makeappend(document* document, size_t line_no, char* str) {
    const char* thisline = document_get_line(document,line_no);
    // fprintf(stdout,"thisline = %s\n",thisline);
    // fprintf(stdout,"thislinep[fin] = %d, strlen= %lu\n",thisline[12],strlen(thisline));

    char* rr = malloc(1000);
    memset(rr,0,1000); 
    strcpy(rr,thisline);
    strcat(rr,str);
    // fprintf(stderr,"????? = %s\n\n",rr);
    document_set_line(document,line_no,rr);
    free(rr); rr = NULL;
}

void handle_append_command(document *document, size_t line_no,
                           const char *line) {
    
    // char* ptr = line;
    size_t nowline = line_no;
    char buf[5000];
    memset(buf,0,sizeof(buf));
    char* bufptr = buf;
    while (*line) {
        // fprintf(stderr,"buf = %s\n",buf);
        if (*line == '\\') {
            if (line[1] == 'n') {
                *bufptr = '\0';
                makeappend(document,nowline,buf);
                document_insert_line(document,++nowline,"");
                // fprintf(stderr,"buf = %s\n",buf);
                memset(buf,0,sizeof(buf));
                bufptr = buf;
                line += 2;
                // nowline++;
            } else if (line[1] == '\\') {
                *bufptr = *line;
                bufptr++;
                line += 2;
            } else {
                line++;
            }
        } else {
            *bufptr = *line;
            bufptr++;
            line++; 
            // fprintf(stderr,"??????????buf = %s\n",buf);  
        }

    }
    // fprintf(stderr,"buf = %s\n",buf);  
    if (*line == '\0') {
        makeappend(document,nowline,buf);
    }
}

void makewrite(document* document, size_t line_no, char* str) {
    const char* thisline = document_get_line(document,line_no);
    char* rr = malloc(1000);
    memset(rr,0,1000); 
    strcpy(rr,thisline);
    size_t i;
    for (i = 0; i < strlen(str); i++) {
        rr[i] = str[i];
    }
    rr[i] = 0;
    
    document_set_line(document,line_no,rr);
    free(rr); rr = NULL;
}

void handle_write_command(document *document, size_t line_no,
                          const char *line) {
    // TODO implement handle_write_command
    // document_set_line()
    size_t nowline = line_no;
    char buf[5000];
    memset(buf,0,sizeof(buf)); 
    char* bufptr = buf;
    while (*line) {
        if (*line == '\\') {
            if (line[1] == 'n') {
                *bufptr = '\0';
                makewrite(document,nowline,buf);
                document_insert_line(document,++nowline,"");
                // fprintf(stderr,"buf = %s\n",buf);
                memset(buf,0,sizeof(buf));
                bufptr = buf;
                line += 2;
                // nowline++;
            } else if (line[1] == '\\') {
                *bufptr = *line;
                bufptr++;
                line += 2;
            } else {
                line++;
            }
        } else {
            *bufptr = *line;
            bufptr++;
            line++; 
            // fprintf(stderr,"??????????buf = %s\n",buf);  
        }

    }
    if (*line == '\0') {
        makewrite(document,nowline,buf);
    }

}
void handle_delete_command(document *document, location loc, size_t num_chars) {
    // TODO implement handle_delete_command
    char buf[5000];
    memset(buf,0,sizeof(buf));
    strcpy(buf,document_get_line(document,loc.line_no));
    char* start = buf + loc.idx;
    char* end;
    if (loc.idx >= strlen(buf)) return;
    if (loc.idx + num_chars > strlen(buf)) {
       *start = '\0';
       char* rr = malloc(strlen(buf)+1);
       strcpy(rr,buf);
       document_set_line(document,loc.line_no,rr);
       free(rr); rr = NULL;
    } else {
        end = buf + loc.idx + num_chars;
        char t1[1000]; char t2[1000];
        *start = '\0';
        strcpy(t1,buf);
        strcpy(t2,end);
        strcat(t1,t2);
        char* rr = malloc(strlen(t1)+1);
        strcpy(rr,t1);
        document_set_line(document,loc.line_no,rr);
        free(rr); rr = NULL;       
    }


}

void handle_delete_line(document *document, size_t line_no) {
    // TODO implement handle_delete_line
    document_delete_line(document,line_no);
}

location handle_search_command(document *document, location loc,
                               const char *search_str) {
    // TODO implement handle_search_command 
    //PASS!
    location rr;
    size_t thisline = loc.line_no;
    size_t idx = loc.idx;
    char buf[1000];
    memset(buf,0,sizeof(buf));
    char* find;
    // int find_bef = 0; 
    strcpy(buf,document_get_line(document,thisline)+idx);
    if ((find = strstr(buf,search_str))) {
        rr.line_no = thisline;
        rr.idx = find - buf + idx;
        return rr;
    } // can be found after cursor this line;
    size_t i = thisline + 1;
    size_t size = document_size(document);
    for (; i <= size; i++) {
        memset(buf,0,sizeof(buf));
        strcpy(buf,document_get_line(document,i));
        if ((find = strstr(buf,search_str))) {
            rr.line_no = i;
            rr.idx = find - buf;
            return rr;
        } 
    }
    i = 1;
    for (;i < thisline; i++) {
        memset(buf,0,sizeof(buf)); 
        strcpy(buf,document_get_line(document,i));
        if ((find = strstr(buf,search_str))) {
            rr.line_no = i;
            rr.idx = find - buf;
            return rr;
        }         
    }
    memset(buf,0,sizeof(buf));
    strcpy(buf,document_get_line(document,thisline));
    if ((find = strstr(buf,search_str))) {
           rr.line_no = thisline;
           rr.idx = find - buf;
           return rr;
    }
    rr.line_no = 0;
    return rr;

}

void handle_merge_line(document *document, size_t line_no) {
    const char* t1 = document_get_line(document,line_no);
    const char* t2 = document_get_line(document,line_no+1);
    char* ret = malloc(1000);
    memset(ret,0,1000);
    strcat(ret,t1);
    strcat(ret,t2);
    document_set_line(document,line_no,ret);
    document_delete_line(document,line_no+1);
    free(ret);

}

void handle_split_line(document *document, location loc) {
    // TODO implement handle_split_line
    char buf[1000];
    memset(buf,0,sizeof(buf));
    size_t line_no = loc.line_no;
    size_t idx = loc.idx;
    strcpy(buf,document_get_line(document,line_no)+idx);
    char* rr = malloc(strlen(buf)+1);
    strcpy(rr,buf);
    document_insert_line(document,line_no+1,rr);
    free(rr);
    memset(buf,0,sizeof(buf));
    strcpy(buf,document_get_line(document,line_no));
    rr = malloc(strlen(buf)+1);
    strcpy(rr,buf);
    rr[idx] = '\0';
    document_set_line(document,line_no,rr);
    free(rr);
}

void handle_save_command(document *document, const char *filename) {
    // TODO implement handle_save_command
    document_write_to_file(document,filename);
}
