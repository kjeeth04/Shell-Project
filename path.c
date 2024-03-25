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

/* Function to attempt running a program given a command that is not a built-in command. No return type
    @param input : index 0 will be the absolute path, and the rest of the indices will contain additional arguments
    @param ioflag : lets the program to redirect input (0), redirect output (1), both (2), or neither (-1)
    @param ioFile : gives name of specified file to redirect input or output to
    @param ioFile2 : used when there is both input and output redirection, and represents where to redirect output
    @param backgroundFlag : checks whether to wait for child process to complete, or to let it run in the background
*/
void path(char** input, int ioflag, char* ioFile, char* ioFile2, int backgroundFlag){
    //Forks process to keep shell running
    fflush(0);
    int pid = fork();

    //Child Process handles external call
    if (pid == 0){
        //Input Redirection
        int inputre = 0;
        if (ioflag == 0 || ioflag == 2){
            rm_space(ioFile);
            inputre = open(ioFile, O_RDONLY); 
            dup2(inputre, 0);
	        close(inputre);
        }
        //Output Redirection if also Input redirection
        int outputre = 0;
        if (ioflag == 2){
            rm_space(ioFile2);
            outputre = open(ioFile2, O_WRONLY | O_CREAT, 0777); 
            dup2(outputre, 1); 
	        close(outputre);
        }
        //Output Redirection
        else if (ioflag == 1){
            rm_space(ioFile);
            outputre = open(ioFile, O_WRONLY | O_CREAT, 0777);
            dup2(outputre, 1);
            close(outputre);
        } 
    
        //Attempts to run absolute path
        int exe = execv(input[0], input);

        //If absolute path did not work
        if (exe == -1){
            //Getting all possible paths for a program call
            char* p = getenv("PATH");
            int a = 0; int* length = &a;
            char **path = parse(p, ":", length);

            //Looping through all of the paths to see if one works
            for (int i = 0; i < *length; ++i){
                char *command = (char *)malloc(PATH_MAX * sizeof(char*));
                strcat(command, path[i]);
                strcat(command, "/");
                strcat(command, input[0]);
                execv(command, input);
                command = 0;
                free(command);
            }

            //Prints error message, and frees allocated memory
            printf("%s: command not found\n", input[0]);
            free(path);
            exit(0);
        }
    }

    //Forces Parent to wait for child to complete, or let run in background, depending on presence of '&' flag
    else {
        //if there was not a & specified in the argument
        if (backgroundFlag == 0){
            wait(NULL);
        }
        
        else {
            int wstatus;
            waitpid(-1, &wstatus, WNOHANG);
        }
    }
}