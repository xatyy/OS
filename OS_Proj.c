/*********************************************
 *        OS_Proj.c - Anthony Esber          *
 *                                           *
 * Project for the Operating Systems course  *
 *                                           *
 *********************************************/
#include <stdio.h>

#include <stdlib.h>

#include <sys/stat.h>

#include <sys/wait.h>

#include <unistd.h>

#include <string.h>

#include <dirent.h>

#include <errno.h>

#include <stdbool.h>

struct stat buffer;
struct stat buffer_target;

int main(int argc, char ** argv) {

  pid_t child_a, child_b;
  int pfd[2];

  if (argc < 2) {
    printf("Invalid arguments! \n");
    exit(-1);
  }

  remove("score.txt");

  for (int i = 1; i < argc; i++) { // Create pipe in parent process only if file is regular and .c
    bool compile_c = false;
    if (lstat(argv[i], & buffer) == 0 && S_ISREG(buffer.st_mode) && strstr(argv[i], ".c")) {

      if (pipe(pfd) < 0) {
        perror("Pipe creation error\n");
        exit(1);
      }
      compile_c = true;
    }

    child_a = fork(); // Create 1st Child

    int status;

    if (child_a < 0) {
      perror("Error creating child \n");
      exit(0);
    } else if (child_a == 0) {

      if (lstat(argv[i], & buffer) == 0) {
        mode_t m;

        char option[1024];
        char sym[1024];

        m = buffer.st_mode;

        if (S_ISREG(m)) { // Handle menu for each type of file

          printf("File %s is regular file.\n\n", argv[i]);
          printf("Please choose an option for regular file #%d: \n", i);
          printf("    -n (file name)\n");
          printf("    -d (file size/dimension)\n");
          printf("    -h (number of hard links)\n");
          printf("    -m (time of last modification)\n");
          printf("    -a (access rights)\n");
          printf("    -l (create a symlink, link name required*)\n");

          fgets(option, 1024, stdin);

          if (strstr(option, "-n") || strstr(option, "n")) {
            printf("File name: %s \n", argv[i]);
          }

          if (strstr(option, "-d") || strstr(option, "d")) {
            printf("File dimension: %ld \n", buffer.st_size);
          }

          if (strstr(option, "-h") || strstr(option, "h")) {
            printf("Number of hard links: %ld \n", buffer.st_nlink);
          }

          if (strstr(option, "-m") || strstr(option, "m")) {
            printf("Last modified: %ld \n", buffer.st_atime);
          }

          if (strstr(option, "-a") || strstr(option, "a")) {
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

          if (strstr(option, "-l") || strstr(option, "l")) {
            printf("Provide a symlink name: \n");

            scanf("%1024s", sym);

            if (symlink(argv[i], sym) == 0) {
              printf("Symlink %s created successfully! \n", sym);
            } else {
              printf("There was an error creating the symlink.\n");
            }

          }

        } else if (S_ISDIR(m)) {
          printf("File %s is directory.\n", argv[i]);

          printf("Please choose an option for directory #%d: \n", i);
          printf("    -n (dir name)\n");
          printf("    -d (dir size/dimension)\n");
          printf("    -a (access rights)\n");
          printf("    -c (total number of .c files)\n");

          fgets(option, 1024, stdin);

          DIR * dir;
          struct dirent * entry;

          dir = opendir(argv[i]);

          if (dir == NULL) {
            perror("Error opening dir! \n");
            exit(-1);
          }

          if (strstr(option, "-n") || strstr(option, "n")) {
            printf("Dir name: %s \n", argv[i]);
          }

          if (strstr(option, "d") || strstr(option, "-d")) {
            printf("The directory dimension/size is: %ld bytes\n", buffer.st_size);
          }

          if (strstr(option, "-a") || strstr(option, "a")) {
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

          if (strstr(option, "c") || strstr(option, "-c")) {

            int files_number = 0;

            while ((entry = readdir(dir)) != NULL) {
              char names[1024];
              snprintf(names, sizeof(names), "%s/%s", argv[i], entry -> d_name);

              if (lstat(names, & buffer) == -1) {
                perror("\nError getting file st: ");
                exit(1);
              }

              if (S_ISREG(buffer.st_mode) && strstr(entry -> d_name, ".c")) {

                files_number++;

              }
            }
            printf("\nTotal number of .c files in the directory is: %d\n", files_number);
            closedir(dir);
          }

        } else if (S_ISLNK(m)) {
          printf("File %s is symbolic link.\n", argv[i]);

          printf("Please choose an option for sym link #%d: \n", i);
          printf("    -n (link name)\n");
          printf("    -l (delete link)\n");
          printf("    -d (size of link)\n");
          printf("    -z (size of target)\n");
          printf("    -a (access rights for symlink)\n");

          fgets(option, 1024, stdin);

          if (strstr(option, "-n") || strstr(option, "n")) {
            printf("Link name: %s \n", argv[i]);
          }

          if (strstr(option, "-l") || strstr(option, "l")) {
            if (unlink(argv[i]) == 0) {
              printf("Link successfully deleted.");
            } else {
              printf("There was an error deleting the link.");
            }
            exit(EXIT_SUCCESS);
          }

          if (strstr(option, "-d") || strstr(option, "d")) {
            printf("Link size: %ld \n", buffer.st_size);
          }

          if (strstr(option, "-z") || strstr(option, "d")) {
            printf("Target size: ");
            if (!stat(argv[i], & buffer_target)) {
              printf("%ld \n", buffer_target.st_size);
            }
          }

          if (strstr(option, "-a") || strstr(option, "a")) {
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
      } else {
        perror("Error reading file. \n");
        exit(-1);
      }

      exit(EXIT_SUCCESS);

    } else {
      int w = waitpid(child_a, & status, 0); // Wait for 1st child process to terminate
      if (WIFEXITED(status)) {
        printf("The process with PID %d has ended with the exit code %d.\n\n", w, WEXITSTATUS(status));  // Process termination acknowledgement
      } else {
        perror("Error exiting.\n");
        exit(-1);
      }
      child_b = fork(); // Create 2nd child process.
      if (child_b < 0) {
        exit(0);
      } else if (child_b == 0) { // Handle each special case.

        if (strstr(argv[i], ".c")) {
          close(pfd[0]);
          dup2(pfd[1], 1);
          execlp("./compile.sh", "./compile.sh", argv[i], NULL); // Execute compile.sh and input to pipe
        } else if (lstat(argv[i], & buffer) == 0) {
          if (S_ISDIR(buffer.st_mode)) {
            char * file_name;
            file_name = strtok(argv[i], "/");
            strcat(file_name, "_file.txt");
            execlp("touch", "touch", file_name, NULL); // Create .txt file with the dir name
            exit(EXIT_SUCCESS);
          } else if (S_ISLNK(buffer.st_mode)) {
            if (execlp("chmod", "chmod", "u=rwx,g=rw,o-rwx", argv[i], NULL) == -1) { // Run chmod on the symlink target
              perror(strerror(errno));
              exit(errno);
            }
            exit(EXIT_SUCCESS);
          } else if (S_ISREG(buffer.st_mode)) {
            int n_lines = 0;
            char buff[1024];
            FILE * f;
            f = fopen(argv[i], "r");
            while (fgets(buff, 1023, f)) {
              n_lines++;
            }   
            printf("%d lines have been read", n_lines); // Read lines of a S_ISREG == true file
          }
        }
        exit(EXIT_SUCCESS);
      }

      w = waitpid(child_b, & status, 0); // Wait for 2nd child process to terminate

      if (WIFEXITED(status)) {
        printf("The process with PID %d has ended with the exit code %d.\n\n", w, WEXITSTATUS(status)); // Process termination acknowledgement
      } else {
        perror("Error exiting.\n");
        exit(-1);
      }
      if (compile_c == true) {
        close(pfd[1]);
        char string[1024];
        while (read(pfd[0], string, 1024)) {}
        close(pfd[0]);

        int err;
        int warn;

        sscanf(string, "%3d\n%3d", & err, & warn);

        int score = 0;

        if (err > 300) {
          printf("! There might be something wrong with compile.sh script. Please check the access rights and make sure it can execute ! \n"); // If err is too big because of garbage values print this warning
        }

        if (err == 0 && warn == 0) {
          score = 10;
        } else if (err > 0) {
          score = 1;
        } else if (warn > 10) {
          score = 2;
        } else if (warn <= 10) {
          score = 2 + 8 * (10 - warn) / 10;
        }

        FILE * out_score;

        out_score = fopen("score.txt", "a");

        fprintf(out_score, "%s:%d \n", argv[i], score);
        compile_c = false;
      }

      fflush(NULL); // Quick fix for parent process re-running more than it should have.

    }

  }

  return 0;

}