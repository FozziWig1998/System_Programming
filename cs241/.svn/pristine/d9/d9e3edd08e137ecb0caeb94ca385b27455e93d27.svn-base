/**
 * Lab: Pointers Gone Wild
 * CS 241 - Fall 2017
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "callbacks.h"
#include "part3-functions.h"
#include "part3-utils.h"
#include "vector.h"

void dragon_destructor(void *elem) {
    if (elem) {
        dragon *d = (dragon *)elem;
        free(d->name);
        free(d);
    }
}

/**
 * Create a vector with 'count' unique dragons.
 * Be sure to check that each dragon has a different name!
 */
vector *one(size_t count) {
    vector *v = vector_create(shallow_copy_constructor, shallow_destructor,
                              shallow_default_constructor);
	
	dragon * new_dragon;
    for (size_t i = 0; i < count; i++) {
    	new_dragon = malloc(sizeof(dragon));
        new_dragon->name = gen_new_dragon_name(NAME_LENGTH);
        new_dragon->talon = id_talon;
        vector_push_back(v, new_dragon);
    }
    return v;
}

/**
 * Join a vector of strings together and return the result.
 * The resulting string should be allocated on the heap.
 *
 * Pseudocode example:
 * two(["foo", "bar", "baz"]) == "foobarbaz"
 */
char *two(vector *vec) {
    // TODO implement me
    char* ret = malloc(sizeof(char) * 100000);
   	char a[100000] = "";
   	memset(a, '\0', sizeof(a));
    size_t i = 0;
    for (i = 0; i < vector_size(vec); i++) {
    	strcat(a,*(vector_at(vec,i)));
    }
    strcpy(ret,a);
    
    return ret;
}

/**
 * Encodes a string into a vector of chunks of no more than 256 bytes.
 * Make sure to use dragon->talon to perform the encoding.
*/
vector *dragon_encode(dragon *dragon, const char *str) {
    // TODO implement me
   // printf("%lu\n\n\n\n",strlen(str));
 	int counter = 0;
	char* p = NULL;
	char* ptr = p;
	vector* retval = string_vector_create();
 	while (*str) {
		if (counter % 256 == 0 ) {
			//if (p) printf("counter is %d, string is %s\n******************************************************\n",counter,p);
			if (p) vector_push_back(retval,p);
			p = (char*)malloc(257 * sizeof(char));
			ptr = p;
			p[256] = '\0';
			
		}
		char chr = dragon->talon(*str);
		//printf("chr is %c\n",chr);
		*ptr = chr;
		ptr++;
		str++;
		counter++;
 	} 	
 	*ptr = '\0';
 	vector_push_back(retval,p);
 	//char* tttttt = "aaaaaaa";
 	//vector_set(retval,0,tttttt);
 	//printf("counter is %d, string final is %s\n\n",counter,p);
 	//printf("%s <<<<<<<<<<<<\n",*vector_at(retval,0));   
    return retval;
}

/**
 * Performs the inverse operation of dragon_encode.
 * The resulting string should be allocated on the heap.
 *
 * Pseudocode: dragon_decode(dragon, dragon_encode(dragon, foo)) == foo
 * Hint: You are allowed to modify the input vector if you wish.
 */
char decoder_fff(char c) {
	c = c ^ 3;
	c = c - 1;
	return c;
}



char *dragon_decode(dragon *dragon, vector *encoded_data) {
    // TODO implement me
    char* ret = two(encoded_data);
    char* ptr = ret;
    while (*ptr) {
    	if (dragon->talon('x') == 'x' && dragon->talon('?') == '?') {
    		*ptr = *ptr;
    	} else if ((dragon->talon('x') ^ 5) == 'x'){
    		*ptr = *ptr ^ 5;
    	} else {
    		*ptr = decoder_fff(*ptr);	
    	}	
    	ptr++;
    }

    
    return ret;
}

char encoder_provided(char c) {
    c = c + 1;
    c = c ^ 3;
    return c;
}



/**
 * Creates and returns an array of two dragons.
 * The second dragon should perform the inverse operation of the first.
 */
dragon *create_dragons() {
    dragon *dragons = malloc(2 * sizeof(dragon));
    dragons[0].name = "Dragon1";
    dragons[1].name = "Dragon2";
    // TODO set the 'talons' of dragons 1 and 2
    // dragon 1 should have the encoder, and 2 should have the decoder
    // To do this, you are going to need
    // to create a function that is the
    // opposite of encoder_provided.
	dragons[0].talon = encoder_provided;
	dragons[1].talon = decoder_fff;
	
//	printf("%c, %c\n",encoder_provided('x'),decoder_fff(encoder_provided('x')));
	
	
    // This will probably make sense if you've implemented encode
    // and decode
    return dragons;
}
