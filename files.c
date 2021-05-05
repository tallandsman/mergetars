#include "mergetars.h"
#define A_SIZE        1000
int i = 0;
FILES fileOrder[A_SIZE];
FILES *file = fileOrder;

//finds and return the relative path names of each file, is used to compare files later on 
char* relativepath(char fullpath[]) {
    int fullpathlen = strlen(fullpath);
    int templen = 20;
    char *relativepath = malloc(fullpathlen + 1);

    for (int i =0 ; i < fullpathlen; ++i){
        if(i>=templen){
            //rearrange the paths name 
            relativepath[i-templen] = fullpath[i];
        }
    }
    return relativepath;
}

/*find all of the files in a directory, stores the files information into a data structure
if the path name is the same then compares it and store the one according to the project specs */
void find_files(char *dirName, int numArg){

    char fullpath[MAXPATHLEN];
    char path[MAXPATHLEN];

    DIR             *dirp;
    struct dirent   *dp;

    dirp       = opendir(dirName);
    if(dirp == NULL) {
        perror("opendir() error");
        exit(EXIT_FAILURE);
    }

        while((dp = readdir(dirp)) != NULL) {
        struct stat  file_info;

        sprintf(fullpath, "%s/%s", dirName, dp->d_name );
        
        //ignoring .DS_Store that are created in macOS
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0 && strcmp(dp->d_name, ".DS_Store") != 0)
        {
            //Construct new path
            strcpy(path, dirName);
            strcat(path, "/");
            strcat(path, dp->d_name);
            
            if(stat(fullpath, &file_info) != 0) {
                 perror("couldn't open stat()");
            }
            else if( S_ISREG( file_info.st_mode )){

                char *relativepath1 = relativepath(fullpath);

                //compare files that have the same relative pathnames and are from different directories
                //only stores the one needed for the output tar 
                for(int c = 0; c < i; c++){
                    file = &fileOrder[c]; 
                    char *relativepath2 = relativepath(file->pathName); //gets the relative pathname of the file

                    if(strcmp(relativepath1, relativepath2) == 0){
                        if(difftime(file->mod_time, file_info.st_mtime) == 0){
                            if(file->size - file_info.st_size == 0){
                                if(file->numArg < numArg){
                                    for(int a = c; a <= i; a++){
                                    fileOrder[a] = fileOrder[a+1]; //deletes the file that's no longer needed
                                    }
                                }
                            }
                        } else if(difftime(file->mod_time, file_info.st_mtime) < 0){ //comparing time
                            for(int a = c; a <= i; a++){
                                fileOrder[a] = fileOrder[a+1];
                            }
                        } else if(file->size - file_info.st_size < 0){ //comparing size
                            for(int a = c; a <= i; a++){
                                fileOrder[a] = fileOrder[a+1];
                            }
                        }
                        i--;
                    }
                }
                
                //stores the file information into the data structure
                file->size = file_info.st_size;
                strcpy(file->relativePath, dp->d_name);
                sprintf(file->pathName, "%s/%s", dirName, dp->d_name );
                file->mod_time = file_info.st_mtime;
                file->numArg = numArg;
                ++i;

            }
            else if(S_ISDIR(file_info.st_mode)) {
                sprintf(path, "%s/%s", dirName, dp->d_name);
                find_files(path, numArg);
            }
            else {
                printf( "%s is unknown!\n", fullpath );
            }
        }
        
    }
    closedir(dirp);
}

//copy all the files we found into to output (temp) directory
void copy_files(char *dest){
    char   buffer[10000];
    size_t got;

    struct timeval newtimes[2];

    int n = 0;

    // open each files and copies it into the output directory
    for(int c = 0; c < i; c++){
        file = &fileOrder[n];

        newtimes[0].tv_sec     = file->mod_time;
        newtimes[0].tv_usec    = 0;
        newtimes[1].tv_sec     = file->mod_time;
        newtimes[1].tv_usec    = 0;

        char filePath[MAXPATHLEN];
        sprintf(filePath, "%s/%s", dest, file->relativePath );
        printf("%s\n", filePath);

        int fd1 = open(filePath, O_RDWR | O_CREAT,0777);
        if(fd1 == -1) {
            perror("cannot open");
            break;
            //exit(EXIT_FAILURE);
        }

        n++;
        int fd0 = open(file->pathName, O_RDONLY);

        if(fd0 == -1) {
            perror("");
            exit(EXIT_FAILURE);

        while((got = read(fd0, buffer, sizeof buffer)) > 0) { 
        if(write(fd1, buffer, got) != got){  
            perror("");
            exit(EXIT_FAILURE);
        }

        // modifies the file time into it's original modification time
        int t = utimes(filePath,newtimes);

        if(t == -1){
            perror("");
            exit(EXIT_FAILURE);
        }

        close(fd0);
        }
    }
    close(fd1);
    }
}

// clean up the directory contents and delete it afterwards
void remove_dir(char *dirName){
    char path[MAXPATHLEN];

    DIR             *dirp;
    struct dirent   *dp;

     struct stat  file_info;

    dirp       = opendir(dirName);
    if(dirp == NULL) {
        perror("opendir() error");
        exit(EXIT_FAILURE);
    }
    while((dp = readdir(dirp)) != NULL) {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
            //Construct new path
            strcpy(path, dirName);
            strcat(path, "/");
            strcat(path, dp->d_name);

            if(stat(path, &file_info) != 0) {
                 perror("couldn't open stat()");
            }
            else if( S_ISDIR( file_info.st_mode )) {
                // recursive call to remove the subdiectories 
                remove_dir(path);
            }
            else if( S_ISREG( file_info.st_mode )) {
                if (unlink(path) == 0) {
                    //continue
                } else {
                    printf("Can`t remove a file: %s\n", path); 
                }
            }
        }
    }
    
    if (rmdir(dirName) == 0){
        //continu
    } else {
        printf("Can`t remove a directory: %s\n", path);
    }

    closedir(dirp);
}
