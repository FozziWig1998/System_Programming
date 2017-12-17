/**
 * Mini Valgrind Lab
 * CS 241 - Fall 2017
 */

#include "mini_valgrind.h"
#include <stdio.h>
#include <string.h>

void *mini_malloc(size_t request_size, const char *filename,
                  void *instruction) {
    // your code here
    //fprintf(stderr,"what?\n");
    meta_data* ptr1 = malloc(sizeof(meta_data) + request_size);
    ptr1->request_size = request_size;
    ptr1->filename = filename;
    ptr1->instruction = instruction;
    ptr1->next = head;
    head = ptr1;
   // fprintf(stderr,"line 21what?\n");
    total_memory_requested += request_size;
    //fprintf(stderr,"line 2331what?\n");
    return ptr1+1;
}

void *mini_calloc(size_t num_elements, size_t element_size,
                  const char *filename, void *instruction) {
    // your code here
	meta_data* ptr1 = malloc(sizeof(meta_data) + num_elements * element_size);
	memset(ptr1,0,sizeof(meta_data) + num_elements * element_size);
	ptr1->request_size = num_elements * element_size;
 	ptr1->filename = filename;	
    ptr1->instruction = instruction;
    ptr1->next = head;
    head = ptr1;
    total_memory_requested += num_elements * element_size;
    return ptr1 + 1; 
}

void *mini_realloc(void *payload, size_t request_size, const char *filename,
                   void *instruction) {
    // your code here
  /*  if (payload == NULL) return mini_malloc(request_size,filename,instruction);
    
    meta_data* ptr1 = (meta_data*)payload - 1;
    int orig_size = ptr1->request_size;
	int dsize = request_size - orig_size;
	    
	mini_free(payload); 
	return mini_malloc(request_size,filename,instruction);
   */     
   	if (payload == NULL) return mini_malloc(request_size,filename,instruction);
   	meta_data* ptr = (meta_data*)payload - 1;
   	int orig_size = ptr->request_size;
	int dsize = request_size - orig_size;
 	meta_data* i = head;
 	meta_data* iprev = NULL;
 	while (i != NULL) {
 		
 		if (ptr == head) {
 			//total_memory_freed += ptr->request_size;
 			head = head->next;
 			free(ptr);
 			break;
 		} else if (ptr == i) {
 			iprev->next = i->next;
 			//total_memory_freed += ptr->request_size;
 			free(ptr);
 			break;
 		}
 		iprev = i;
 		i = i->next;
 	} 
 	
 	if (i == NULL) {
 		++invalid_addresses;
		return NULL;
	}	
	
	meta_data* ptr1 = malloc(sizeof(meta_data) + request_size);
    ptr1->request_size = request_size;
    ptr1->filename = filename;
    ptr1->instruction = instruction;
    ptr1->next = head;
    head = ptr1;
   // fprintf(stderr,"line 21what?\n");
   // total_memory_requested += request_size;
	if (dsize > 0) total_memory_requested += dsize;
	else total_memory_freed -= dsize;	
	return ptr1+1;
   
}

void mini_free(void *payload) {
	if (payload == NULL) return;
	if (head == NULL) {
		invalid_addresses++;
		return;
	}	
	meta_data* ptr = (meta_data*)payload - 1;
 	meta_data* i = head;
 	meta_data* iprev = NULL;
 	while (i != NULL) {
 		
 		if (ptr == head) {
 			total_memory_freed += ptr->request_size;
 			head = head->next;
 			free(ptr);
 			return;
 		} else if (ptr == i) {
 			iprev->next = i->next;
 			total_memory_freed += ptr->request_size;
 			free(ptr);
 			return;
 		}
 		iprev = i;
 		i = i->next;
 	} 
 	if (i == NULL) ++invalid_addresses;
}

