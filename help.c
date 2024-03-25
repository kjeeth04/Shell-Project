#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "helpers.h"

//Function to display all available commands. No parameters or return type
void help(){
    //Opens designated help.txt file using absolute path
    FILE *help;
    help = fopen("/home/TU/tup61179/3207/Lab1/help.txt", "r");

    //Displays file contents
    int c; 
    while ((c = fgetc(help)) != EOF) { 
        printf("%c", c);
    }
    printf("\n");
    //Closes help.txt
    fclose(help);
}