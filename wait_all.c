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

/*Function to wait for all background processes to stop
    @param backgroundCount : pointer to the backgroundCount variable. Used to decrease the value by reference
*/
void wait_all(int* backgroundCount){
    //Waits for all child processes to finish
    while (*backgroundCount > 0){
        wait(NULL);
        --(*backgroundCount);
    } 
    printf("All background processes complete\n");
}