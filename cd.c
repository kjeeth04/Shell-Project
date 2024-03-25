#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/limits.h>
#include <errno.h>
#include "helpers.h"

/*Function to change the current directory. Does not return anything
    @param loc : the specified directory to move to

    Will throw an error if a valid directory location is not provided
*/
void cd(char *loc){
    int test = chdir(loc);
    
    //Handling if specified directory does not exist or if specified directory is not a directory
    if (test == -1){
        int err = errno;
        if (err == ENOENT){
            printf("%s: No such file or directory\n", loc);
        }
        else if (err == ENOTDIR){
            printf("%s: Not a directory\n", loc);
        }
    }
}