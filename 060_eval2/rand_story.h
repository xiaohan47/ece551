#ifndef __RAND_STORY_H__
#define __RAND_STORY_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "provided.h"
//any functions you want your main to use

FILE * open_file(const char * file_name);
//read the temp file and return a string of temp file
char * read_temp(FILE * f);

//parse the temp file and return the parsed reult string
char * parse_temp(char * temp, catarray_t * cats, int flag);

//read the list file and creat the catarray_t for it.
catarray_t * read_list(FILE * f);

void free_cat(catarray_t * cat);

#endif
