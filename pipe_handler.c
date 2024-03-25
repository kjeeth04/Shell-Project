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

/*Function to handle input when it contains pipes. No return type
    @param pipeargs : array of strings containing the arguments that will be piped to each other
    @param backgroundFlag : determines whether or not the piping will occur in the background or not
    @param pipelength : amount of commands to be piped
*/
void pipe_handler(char** pipeargs, int pipelength, int backgroundFlag){
    //First Child Process is where all piping will occur inside
    int pid1 = fork();

    if (pid1 == 0){
        //Creating pipes to send data between processes
        int pipes[pipelength-1][2];

        for (int i = 0; i < pipelength; ++i){
            //Piping each int[] as needed
            if (i != pipelength-1){
                pipe(pipes[i]);
            }

            int pid2 = fork();
            //Second Child Process(es) where individual iterations of program execution and piping will take place within

            if (pid2 == 0){
                //Switch STDIN with read end of pipeFrom if not on the first iteration
                if (i != 0){
                    dup2(pipes[i-1][0], 0);
                    close(pipes[i-1][0]);
                }

                //Switches STDOUT to write end of pipeTo if not at the last iteration and close pipeTo
                //The last iteration should output to STDOUT so it is excluded
                if (i != pipelength-1){
                    dup2(pipes[i][1], 1);
                    close(pipes[i][1]);
                }

                //Breaks up argument and attempts to run it in a manner similar to path.c
                int a = 0; int* placeholder = &a;
                char **input = parse(pipeargs[i], " \n", placeholder);

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
                free(input);
                exit(0);
                }
            }

            //Forces First Child to wait for each iteration of Second Child to complete
            else {
                //Closing write to end of current pipe, and read from end of previous pipe
                if (i != 0){
                    close(pipes[i-1][0]);
                }
                if (i != pipelength-1){
                    close(pipes[i][1]);
                }  
                wait(NULL);
            }
        }  

            //Exits the First Child
            exit(0);
    }
    //Forces Parent to wait for First Child to complete, or let run in background, depending on presence of '&' flag
    else {
            
        //if there was not a & specified in the argument
        if (backgroundFlag == 0){
            wait(NULL);
        }
        //If there was a & specified in the argument
        else {
            int wstatus;
            waitpid(-1, &wstatus, WNOHANG);
        }
    }    
}