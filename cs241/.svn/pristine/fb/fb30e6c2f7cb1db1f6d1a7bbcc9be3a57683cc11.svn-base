/**
 * Machine Problem: Text Editor
 * CS 241 - Fall 2017
 */
#include "document.h"
#include "editor.h"
#include "format.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * You can programatically test your text editor.
*/
int main() {
    // Setting up a docment based on the file named 'filename'.
    char *filename = "test.txt";
    document *doc = document_create_from_file(filename);

    // print the first 5 lines
   	 // handle_display_command(doc, 1, 10, 0, -1);
    // printf("strlen line first = %lu\n",strlen(document_get_line(doc,5)) );
      location loc;
      loc.idx = 1;
      loc.line_no = 1;
 	 char* test1 = "peanuts ";
	 handle_insert_command(doc,loc,test1); 

	// handle_display_command(doc, 1, 10, 0, -1);
	// // handle_display_command(doc, 1, 10, 0, -1);
	// // loc = handle_search_command(doc,loc,"WYH");
	// // printf("line_no = %lu, idx = %lu\n",loc.line_no,loc.idx);
 //    // handle_display_command(doc, 1, 10, 0, -1);

 //    // printf("\n");

//   handle_merge_line(doc,2);

 //  	 // handle_display_command(doc, 1, 10, 0, -1);
 //  	 // printf("\n");

	// // handle_display_command(doc, 1, 10, 0, -1);
	
	// loc.line_no = 2;
	// loc.idx = 7;
 //   // printf("TEST split\n");
	// handle_split_line(doc,loc);
 // // handle_display_command(doc, 1, 10, 0, -1);
 //  	 // printf("\n");
	// // handle_display_command(doc, 1, 10, 0, -1);
     // printf("TEST doc insert\n");

	 // document_insert_line(doc,1,"shen me gui?");

	// loc.line_no = 2;
	// loc.idx = 3;
	//  // printf("TEST insert commd\n");
	//  handle_insert_command(doc,loc,"~~test insert~~");
	

	// // handle_display_command(doc, 1, 15, 0, -1);

	 //  printf("TEST append commd\n");

	 // handle_append_command(doc,3,"abc\\nde\\fg\\\\ncccc\\n");

	//  handle_display_command(doc, 1, 15, 0, -1);

	// printf("strlen line = %lu\n",strlen(document_get_line(doc,6)) );
 // printf("TEST wr commd\n");
	// handle_write_command(doc,3,"Yes\\nNo");

	handle_display_command(doc, 1, 15, 0, -1);

	document_destroy(doc);

}
