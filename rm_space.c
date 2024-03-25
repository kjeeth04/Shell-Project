#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/limits.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h> 
#include "helpers.h"

/*Function to remove white space from a string. No return type
    @param s : string to be targetted
*/
void rm_space(char *s){
    int counter = 0;
    for (int i = 0; s[i]; i++)
        if (s[i] != ' '){
            s[counter++] = s[i];
        }
    s[counter] = '\0';
}