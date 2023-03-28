#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

struct stat buffer;

int main(int argc, char ** argv){
    
    if(argc < 2){
        printf("Invalid arguments! \n");
        exit(-1);
    }

    

    for(int i = 1; i < argc; i++){
        if(lstat(argv[i], &buffer) == 0){
            mode_t m;
            char option[1024];

            m = buffer.st_mode;
            
            if(S_ISREG(m)){
                printf("File #%d is regular file.\n", i);
                fgets(option, 1024, stdin);
  
            }else if(S_ISDIR(m)){
                printf("File #%d is directory.\n", i);
            }else if(S_ISLNK(m)){
                printf("File #%d is symbolic link.\n", i);
            }
        }else{
            printf("Error reading file \n");
            exit(-1);
        }
        
    }

    return 0;

}