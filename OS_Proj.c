#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

struct stat buffer;
struct stat buffer_target;

int main(int argc, char ** argv){
    
    if(argc < 2){
        printf("Invalid arguments! \n");
        exit(-1);
    }

    

    for(int i = 1; i < argc; i++){
        int pid, status;

            if((pid=fork()) < 0){
                exit(0);
            } 
            
            else if(pid==0){
        if(lstat(argv[i], &buffer) == 0){
            mode_t m;

            char option[1024];
            char sym[1024];

            m = buffer.st_mode;

            
            if(S_ISREG(m)){
                if(strstr(argv[i],".c")){
                    printf("Argument is a .c file.\n");
                    pid = fork();

                    if(pid < 0){
                        exit(0);
                    }else if(pid == 0){
                        execlp("./compile.sh", "./compile.sh",argv[i], NULL);
                        exit(0);
                    }else{
                        int status;
                        wait(&status);
                    }

                }
                printf("File #%d is regular file.\n\n", i);
                printf("Please choose an option for regular file #%d: \n", i);
                printf("    -n (file name)\n");
                printf("    -d (file size/dimension)\n");
                printf("    -h (number of hard links)\n");
                printf("    -m (time of last modification)\n");
                printf("    -a (access rights)\n");
                printf("    -l (create a symlink, link name required*)\n");

                fgets(option, 1024, stdin);

                if(strstr(option, "-n") || strstr(option, "n")){
                    printf("File name: %s \n", argv[i]);
                }

                if(strstr(option, "-d") || strstr(option, "d")){
                    printf("File dimension: %ld \n", buffer.st_size);
                }

                if(strstr(option, "-h") || strstr(option, "h")){
                    printf("Number of hard links: %ld \n", buffer.st_nlink);
                }


                if(strstr(option, "-m") || strstr(option, "m")){
                    printf("Last modified: %ld \n", buffer.st_atime);
                }

                if(strstr(option, "-a") || strstr(option, "a")){
                    printf("Access Rights:");
                    printf("\nOWNER:");
                    printf("\n  Read:");
                    printf((m & S_IRUSR) ? "YES \n" : "NO \n");
                    printf("  Write:");
                    printf((m & S_IWUSR) ? "YES \n" : "NO \n");
                    printf("  Exec:");
                    printf((m & S_IXUSR) ? "YES \n" : "NO \n");

                    printf("\nGROUP:");
                    printf("\n  Read:");
                    printf((m & S_IRGRP) ? "YES \n" : "NO \n");
                    printf("  Write:");
                    printf((m & S_IWGRP) ? "YES \n" : "NO \n");
                    printf("  Exec:");
                    printf((m & S_IXGRP) ? "YES \n" : "NO \n");

                    printf("\nUSERS:");
                    printf("\n  Read:");
                    printf((m & S_IROTH) ? "YES \n" : "NO \n");
                    printf("  Write:");
                    printf((m & S_IWOTH) ? "YES \n" : "NO \n");
                    printf("  Exec:");
                    printf((m & S_IXOTH) ? "YES \n" : "NO \n");
                }

                if(strstr(option, "-l") || strstr(option, "l")){
                    printf("Provide a symlink name: \n");

                    fgets(sym, 1024, stdin);

                    if(symlink(argv[i], sym) == 0){
                        printf("Symlink %s created successfully! \n", sym);
                    }else{
                        printf("There was an error creating the symlink.\n");
                    }

                }
                
            }else if(S_ISDIR(m)){
                printf("File #%d is directory.\n", i);

                printf("Please choose an option for directory #%d: \n", i);
                printf("    -n (dir name)\n");
                printf("    -d (dir size/dimension)\n");
                printf("    -a (access rights)\n");
                printf("    -c (total number of .c files)\n");

                fgets(option, 1024, stdin);

                DIR *dir;
                struct dirent *entry;

                dir = opendir(argv[i]);  

                if(dir == NULL){
                    printf("Error opening dir! \n");
                    exit(-1);
                }

                if(strstr(option, "-n") || strstr(option, "n")){
                    printf("Dir name: %s \n", argv[i]);
                }

                if(strstr(option, "d") || strstr(option, "-d")){
                printf("The directory dimension/size is: %ld bytes\n", buffer.st_size);
                }

                if(strstr(option, "-a") || strstr(option, "a")){
                    printf("Access Rights:");
                    printf("\nOWNER:");
                    printf("\n  Read:");
                    printf((m & S_IRUSR) ? "YES \n" : "NO \n");
                    printf("  Write:");
                    printf((m & S_IWUSR) ? "YES \n" : "NO \n");
                    printf("  Exec:");
                    printf((m & S_IXUSR) ? "YES \n" : "NO \n");

                    printf("\nGROUP:");
                    printf("\n  Read:");
                    printf((m & S_IRGRP) ? "YES \n" : "NO \n");
                    printf("  Write:");
                    printf((m & S_IWGRP) ? "YES \n" : "NO \n");
                    printf("  Exec:");
                    printf((m & S_IXGRP) ? "YES \n" : "NO \n");

                    printf("\nUSERS:");
                    printf("\n  Read:");
                    printf((m & S_IROTH) ? "YES \n" : "NO \n");
                    printf("  Write:");
                    printf((m & S_IWOTH) ? "YES \n" : "NO \n");
                    printf("  Exec:");
                    printf((m & S_IXOTH) ? "YES \n" : "NO \n");
                }

                if(strstr(option, "c") || strstr(option, "-c")){

                    int files_number = 0;

                    while((entry = readdir(dir)) != NULL){
                        char names[1024];
                        snprintf(names, sizeof(names), "%s/%s", argv[i], entry->d_name);

                        if (lstat(names, &buffer) == -1) {
                            perror("\nError getting file st: ");
                            exit(1);
                        }

                        if(S_ISREG(buffer.st_mode) && strstr(entry->d_name, ".c")){
                            
                            files_number++;

                        }
                    }
                    printf("\nTotal number of .c files in the directory is: %d\n", files_number);
                    closedir(dir);
                }

            }else if(S_ISLNK(m)){
                printf("File #%d is symbolic link.\n", i);

                printf("Please choose an option for sym link #%d: \n", i);
                printf("    -n (link name)\n");
                printf("    -l (delete link)\n");
                printf("    -d (size of link)\n");
                printf("    -z (size of target)\n");
                printf("    -a (access rights for symlink)\n");

                fgets(option, 1024, stdin);

                if(strstr(option, "-n") || strstr(option, "n")){
                    printf("Link name: %s \n", argv[i]);
                }

                if(strstr(option, "-l") || strstr(option, "l")){
                    if(unlink(argv[i]) == 0){
                        printf("Link successfully deleted.");
                    }else{
                        printf("There was an error deleting the link.");
                    }
                }

                if(strstr(option, "-d") || strstr(option, "d")){
                    printf("Link size: %ld \n", buffer.st_size); 
                }

                if(strstr(option, "-z") || strstr(option, "d")){
                    printf("Target size: "); 
                    if(!stat(argv[i], &buffer_target)){
                        printf("%ld \n",buffer_target.st_size);
                    }
                }

                if(strstr(option, "-a") || strstr(option, "a")){
                    printf("Access Rights:");
                    printf("\nOWNER:");
                    printf("\n  Read:");
                    printf((m & S_IRUSR) ? "YES \n" : "NO \n");
                    printf("  Write:");
                    printf((m & S_IWUSR) ? "YES \n" : "NO \n");
                    printf("  Exec:");
                    printf((m & S_IXUSR) ? "YES \n" : "NO \n");

                    printf("\nGROUP:");
                    printf("\n  Read:");
                    printf((m & S_IRGRP) ? "YES \n" : "NO \n");
                    printf("  Write:");
                    printf((m & S_IWGRP) ? "YES \n" : "NO \n");
                    printf("  Exec:");
                    printf((m & S_IXGRP) ? "YES \n" : "NO \n");

                    printf("\nUSERS:");
                    printf("\n  Read:");
                    printf((m & S_IROTH) ? "YES \n" : "NO \n");
                    printf("  Write:");
                    printf((m & S_IWOTH) ? "YES \n" : "NO \n");
                    printf("  Exec:");
                    printf((m & S_IXOTH) ? "YES \n" : "NO \n");
                }
            }
        }else{
            printf("Error reading file \n");
            exit(-1);
        }
        
        exit(EXIT_SUCCESS);
        
    }else{
        waitpid(pid, &status, 0);
    }
        
    }

    return 0;

}