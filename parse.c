#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "helpers.h"

/*
    Function to break up a line (or any arbitrary string) into a memory allocated
    tokenized array of strings separated by the given delimiter or set of 
    delimiters (see strtok_r for more information about delim parameter). 
    
    Note: The array will be memory allocated, therefore they must be freed later. 
    
    Warning: LINE MUST BE MEMORY ALLOCATED!

    @param line : An arbitrary string
    @param delim : The delimiter or set of delimiters for separating 
        the line (see strtok_r for more information)
    @param length : gives length of array of arguments by-reference
    
    @return array : array of pointers to strings in line.
*/
char** parse(char* line, const char delim[], int* length){

    char** array = (char**)malloc(sizeof(char*));
    *array=NULL;
    int n = 0;

    char* buf = strtok(line,delim);

    if (buf == NULL){
        free(array);
        array=NULL;
        return array;
    }

    while (buf != NULL){
        array = (char**)realloc(array,(n+2)*sizeof(char*));
        if(array==NULL){
            free(array);
            array=NULL;
            return array;
        }

        array[n]=buf;
        n++;
        array[n]=NULL;

        buf = strtok(NULL,delim);
    }
    //Getting length of array of argument
    *length = n;
    return array;
}