#include "mergetars.h"
#define TEMPLATE     "/tmp/project2-XXXXXX"

//  CITS2002 Project 2 2020
//  Name(s):             Tal Ashkenazy Landsman (, Callum Paterson)
//  Student number(s):   22981003 (, 22723279)

int main(int argc, char *argv[]){

    //check there's a valid number of arguments
    if(argc < 2) {
        printf("invalid number of arguments\n");
        exit(EXIT_FAILURE);             
    }

    char outputDir[MAXPATHLEN];
    char *tempDirs[argc];

    strcpy(outputDir,TEMPLATE);
    mkdtemp(outputDir);
    printf("output temp dir is %s\n", outputDir);

    for (int z = 0 ; z < argc; z++) {
        if ((tempDirs[z] = malloc(sizeof(char) * 19)) == NULL) {
            printf("unable to allocate memory \n");
            exit(EXIT_FAILURE);
        }
    }

    for(int z = 1; z < argc-1; z++){
       char newDirName[MAXPATHLEN];

       strcpy(newDirName,TEMPLATE);
       mkdtemp(newDirName); 

       strcpy(tempDirs[z-1],newDirName);

       expand_tar(argv[z], newDirName);
       find_files(newDirName, z);

    }
    printf("%d", argc);

    copy_files(outputDir);

    create_tar(outputDir,argv[argc-1]);

    //remove all temporary directoris created
    for(int j = 0; j < argc-1; j++){
        remove_dir(tempDirs[j]);
        free(tempDirs[j]);
    }

    remove_dir(outputDir);

    return 0;
}
