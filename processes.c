#include "mergetars.h"

/* Expands each tar archive into its own temp directory , if it can't be expended then exit
forks a new child, the child process runs the exucution while parent is waiting for it */
void expand_tar(char *fileName, char *dirName){
    switch ( fork() ) {
        case -1 :   printf("failed to fork a process\n"); // error has occured 
                    exit(EXIT_FAILURE);
                    break;

        case 0:    /* executed by child process
                   extracts the contnets of a tar file into the empty directory */
                   execl("/usr/bin/tar", "tar", "-xvf", fileName, "-C", dirName, "--strip-components=1", (char *)NULL);  //"--strip-components=1" to only extact files
                   break;

        default:  {
                  int child, status;                      // executed by parent process
                  printf("parent waiting\n");
                  child = wait( &status );
                  printf("process pid=%i terminated with exit status=%i\n",child, WEXITSTATUS(status));
                  break;
                }
    }

}

//create the result(output) tar file 
void create_tar(char *outputDir, char *outputTar){
        switch ( fork() ) {
        case -1 :   printf("failed to fork a process\n"); // error has occured 
                    exit(EXIT_FAILURE);
                    break;

        case 0:    // executed by child process 
                   execl("/usr/bin/tar", "tar", "-C", outputDir, "-cpf", outputTar,".",(char *)NULL);
                   perror("couldn't execl");
                   break;

        default:  {
                  int child, status;                      // executed by parent process
                  printf("parent waiting\n");
                  child = wait( &status );
                  printf("process pid=%i terminated with exit status=%i\n",child, WEXITSTATUS(status));
                  break;
                }
    }
}
