#ifndef helpers_h
#define helpers_h

char** parse(char* line, const char delim[], int* length);

void exit_shell();

void help();

void pwd();

void cd(char *loc);

void path(char** input, int ioflag, char* ioFile, char* ioFile2, int backgroundFlag);

void rm_space(char *s);

void wait_all(int* backgroundCount);

void pipe_handler(char** pipeargs, int pipelength, int backgroundFlag);

#endif /* helpers_h */