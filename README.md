## Jeeth's Shell

# Make sure to compile code using the provided makefile.
# This program was designed and optimized to compile and execute most efficiently in a Linux/UNIX environment. 
# Compiling or running in a non-Linux/UNIX environment may cause errors
# The 'help' function does not work on systems outside of the Temple University Servers

Core Shell (shell.c) exists and runs indefinitely until 'exit' is called.

Built-in functions 'help', 'exit', 'pwd', and 'cd' are fully implemented and operational.
These functions are implemented via help.c, exit_shell.c, pwd.c, and cd.c respectively

parse() is used to break up input into arrays of individual arguments.

Given an absolute path, such as 'usr/bin/ls', fork() and execv() are used to create a child process that attempts to run the specified program.

If the argument is not recognized as an absolute path, the $PATH environment variable is used to find the absolute path of an argument, and will use strcat() to append every possible path location to the given argument. If execv() does not work on any of these arguments, then an error message will be displayed.

When a '<', '>', or both are detected in the user input, the shell will redirect input and/or output to the specified locations/programs. dup2() is used to redirect input/output.

To combat an issue with whitespace in arguments when doing file redirection, rm_space.c contains a function that removes whitespace from a string.

Built-in function 'wait' is fully implemented and operational, and is implemented via wait.c

If user input into the shell ends with a '&', then program execution will be ran in the background

If a pipe symbol "|" is used, then the output of the command to the left of the pipe will be used as input for the command to the right of the pipe. Infinite piping is supported. Pipe handling is done within pipe_handler.c

# Testing
Testing the build in functions 'help', 'exit', 'pwd', and 'cd' can be done by simply calling them in the shell.

Testing absolute path execution can be tested by inputting an absolute program path, such as 'usr/bin/ls'.
Testing a relative path can be done by testing 'ls'.

Testing input redirection can be done by doing ls > output.txt, which should create a .txt file with a list of all files in the directory.
Testing output redirection can be done by doing cat < output.txt, which should display the contents of output.txt.
Testing simultaneous input and output redirectiom can be done by doing wc < output.txt > output2.txt, which will input information of output.txt into 'wc', which will then output word count information about output.txt into output2.txt

Testing background tasks can be done by doing 'sleep 5 &', which will then be active in the background. All external commands can be ran in the background, and is fully compatible with piping.
Testing wait can be done by calling 'wait' after 'sleep 5 &', which will then cause the shell to wait for 'sleep 5' to complete.

Testing single piping can be done using 'ls | wc', which will output the word count information of 'ls', which contains all the names of all the files in the directory.
Testing multiple piping can be done using 'ls | wc | wc', which will output the word count information of the word count of all the names of all the files in the directory.
