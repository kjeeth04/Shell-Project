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

int main(int argc, char *argv[]){
    //Greeting message and initializing background processes counter
    printf("\n||Welcome to Jeeth's Shell||\n\n");
    printf("Type 'help' for a list of commands\n");
    int backgroundCount = 0;
    while(1){
        //Getting and parsing user input
        printf("\n>>> ");
        char* rawinput = (char *)malloc(ARG_MAX);
        fgets(rawinput, ARG_MAX, stdin);

        //If nothing or just & was inputted
        if (strcmp(rawinput, "\n") == 0 || strcmp(rawinput, "&\n") == 0){
            free(rawinput); 
            continue;
        }
        
        //Checks to see if there is a & at the end of input, which means the task will run in the background
        //if backgroundFlag is 0, then will not run in background, and if it is 1, then it will run the command in the background
        int backgroundFlag = 0;
        if (rawinput[strlen(rawinput)-2] == '&'){
            rawinput[strlen(rawinput)-2] = '\0';
            backgroundFlag = 1;
            ++backgroundCount;
        }

        //Check if Pipe(s) and Input/Output redirection are both present
        if (strchr(rawinput, '|') != NULL && (strchr(rawinput, '>') != NULL || (strchr(rawinput, '<') != NULL))){
            printf("Piping is not compatible with Input or Output Redirection\n");
            free(rawinput);
            continue;
        }

        //Checks to see if pipes exist, and if they do, then pipe_handler() is called and the rest of the code in the shell loop is skipped over
        int d = 0; int* pipelength = &d;
        char** pipeargs = parse(rawinput, "|\n", pipelength);
        if (*pipelength > 1){
            pipe_handler(pipeargs, *pipelength, backgroundFlag);
            free(pipeargs);
            continue;
        }
        //If not, free allocated memory for array of pipe arguments, and continue with the rest of the shell loop
        else {
            free(pipeargs);
        }

        //Check for output redirection
        //ioflag determines if output is being redirected or not (-1: no redirection; 0: input redirection; 1: output redirection; 2: intput and output redirection)
        int b = 0; int* olength = &b; int ioflag = -1;
        char** oinput = parse(rawinput, ">\n", olength);
        if (*olength > 1){
            ioflag = 1;
        }
        //Check for input redirection
        int c = 0; int* ilength = &c;
        char** iinput = parse(oinput[0], "<\n", ilength);
        if (*ilength > 1 && ioflag == 1){
            //Input and Output redirection
            ioflag = 2;
        }
        else if (*ilength > 1){
            //Just Input redirection
            ioflag = 0;
        }


        //Breaks up input into multiple words, with the string used depending on the value of ioflag
        int a = 0; int* length = &a; 
        char *command = (char *)malloc(PATH_MAX * sizeof(char*));
        //If there is Input or Input + Output redirection
        if (ioflag == 0 || ioflag == 2){
            strcat(command, iinput[0]);
        }
        //Otherwise
        else {
            strcat(command, oinput[0]);
        }
        char** input = parse(command, " ", length);
        command = 0;
        free(command);

        //Input for built-in exit
        if (strcmp(input[0], "exit") == 0){
            exit_shell();
        }
        //Input for built-in help, only runs when there are no other arguments
        else if (strcmp(input[0], "help") == 0 && *length == 1){
            int pid = fork();
            //Child Process is created to prevent unforseen issues with stdin and stdout in the parent process
            if (pid == 0){
                help();
                exit(0);
            }
            //Waits for child to complete
            else {
                wait(NULL);
            }
        }
        //Input for built-in pwd
        else if (strcmp(input[0], "pwd") == 0){
            pwd();
        }
        //Input for built-in cd, only runs when there are 2 arguments ('cd' and location)
        else if (strcmp(input[0], "cd") == 0 && *length == 2){
            cd(input[1]);
        }
        else if (strcmp(input[0], "wait") == 0 && *length == 1){
            int *temp;
            temp = &backgroundCount;
            wait_all(temp);
        }
        //Uses $PATH or absolute path to execute all other commands
        else {
            //ioFile and ioFile2 parameters modified based on redirection type
            if (ioflag == 0){
                path(input, ioflag, iinput[1], " ", backgroundFlag);
            }
            else if (ioflag == 1){
                path(input, ioflag, oinput[1], " ", backgroundFlag);
            }
            else if (ioflag == 2){
                path(input, ioflag, iinput[1], oinput[1], backgroundFlag);
            }
            else {
                path(input, ioflag, " ", " ", backgroundFlag);
            }
        }

        //Freeing allocated memory
        free(rawinput);
        free(iinput);
        free(oinput);
        free(input);
    }
}