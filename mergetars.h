#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdarg.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/param.h>  
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/time.h>

//Structure to hold the directory and its content attributes - TO BE CHANGED 
typedef struct{
    char pathName[MAXPATHLEN];
    char relativePath[MAXPATHLEN];
    time_t mod_time;
    int size;
    int numArg;
} FILES;

extern FILES *file;

extern void expand_tar(char *fileName, char *dirName);

extern void create_tar(char *outputDir, char *outputTar);

extern char* relativepath(char fullpath[]);

extern void find_files(char *dirName, int numArg);

extern void copy_files(char *dest);

extern void remove_dir(char *dirName);
