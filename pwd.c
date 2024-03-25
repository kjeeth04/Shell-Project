#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/limits.h>
#include "helpers.h"

//Function to display the path to the current file directory you are in. No parameters or return type
void pwd(){
    //Gets the path
    char path[PATH_MAX];

    getcwd(path, sizeof(path));
    //Prints the path
    printf("%s\n", path);
}