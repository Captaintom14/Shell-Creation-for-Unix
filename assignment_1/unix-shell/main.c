
//MAIN
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h> //for INT_MAX
#include "parser.h"
#include "utils.h"
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <fcntl.h>  

void terminate(char *line) {
    if (line)
        free(line); //release memory allocated to line pointer
    printf("bye\n");
    exit(0);
}

// QUESTION 2 Jobs
struct backgroundJobs{
    pid_t pid; //checks the process id
    int status; // checks the status of the process
    char *command; // checks the command inside of the []
};

struct backgroundJobs bgJobs[1000]; //array of background jobs
int jobCount = 0; //counter for the number of jobs

void checkJobs(){
    for(int i = 0; i < jobCount; i++){
        if (bgJobs[i].status == 0){ // checks if the job is running
           
           int status = 0;
           pid_t stat = waitpid(bgJobs[i].pid, &status, WNOHANG);

              if (stat == 0){
                printf("[%d] %d Running %s\n",i + 1, bgJobs[i].pid, bgJobs[i].command);
              }

              else if (stat == -1 && errno != ECHILD){
                perror("waitpid"); // there's an error while waiting for the process
               }

              else{
                printf("[%d] %d Done %s\n", i + 1,bgJobs[i].pid, bgJobs[i].command);
                bgJobs[i].status = 1; // the job is finished
              }
        }
    
    }

}
// END QUESTION 2

//QUESTION 3 Input/Output Redirection
void redirection(struct cmdline *l){
    int fd;
    if (l->in != 0){
        fd = open(l->in, O_RDONLY);
        if (fd == -1){
            perror("Failed to open 1 file");
            exit(1);
        }
        if (dup2(fd, STDIN_FILENO) == -1){
            perror("Failed to duplicate file descriptor");
            exit(1);
        }
        close(fd);
    }
    if (l->out != 0){
        fd = open(l->out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1){
            perror("Failed to open 2 file");
            exit(1);
        }
        if (dup2(fd, STDOUT_FILENO) == -1){
            perror("Failed to duplicate file descriptor");
            exit(1);
        }
        close(fd);
    }
}

// END QUESTION 3


// QUESTION 4 Simple Pipe Implementation
//This function will be used to verify if the command has a pipe or not
void pipesFunction (char **cmd1, char **cmd2);

void verification (char *input, struct cmdline *l){
    int i = 0;
    char *cmd1[1000];
    char *cmd2[1000];
    char *token = strtok(input, " \n");
    char *outputFile = NULL;
    char *inputFile = NULL;
    int pipeFound = 0;
    int redirectionOut = 0;
    int redirectionIn = 0;
    while (token != NULL){
        if (strcmp(token, "|") == 0){
            pipeFound = 1; // pipe is found in the command
            cmd1[i] = NULL;
            i = 0;
            token = strtok(NULL, " \n");
            continue;
        } 
        if (strcmp(token, ">") == 0){
            redirectionOut = 1; // redirection is found in the command
            cmd1[i] = NULL;
            token = strtok(NULL, " \n");
            if (token != NULL){
                outputFile = token;
            }
            break;
        } 

        if (strcmp (token, "<") == 0){
            redirectionIn = 1; // redirection is found in the command
            cmd1[i] = NULL;
            token = strtok(NULL, " \n");
            if (token != NULL){
                inputFile = token;
            }
            break;
        }

        if (pipeFound == 0){
            cmd1[i++] = token; // read the first command
        } else if (pipeFound == 1){
            cmd2[i++] = token; // read the second command
        } 
        token = strtok(NULL, " \n");
    }

     if (pipeFound == 1){
        cmd1[i] = NULL; // execute the first command which 
        cmd2[i] = NULL; // execute the second command which has the pipe
        pipesFunction(cmd1, cmd2);
     } else {

        cmd1[i] = NULL; // execute the first command which doesn't have the pipe

        if (strcmp(cmd1[0],"exit") == 0){
            printf("bye\n");
            exit(0);
        }

        pid_t pid = fork();
        if (pid == 0){

            if (redirectionOut == 1){
                int fd = open(outputFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd == -1){
                    perror("Failed to open output file");
                    exit(1);
                }
                if (dup2(fd, STDOUT_FILENO) == -1){
                    perror("dup2");
                    exit(1);
                }
                close(fd);

            if (redirectionIn == 1){
                int fd = open(inputFile, O_RDONLY);
                if (fd == -1){
                    perror("Failed to open input file");
                    exit(1);
                }
                if (dup2(fd, STDIN_FILENO) == -1){
                    perror("dup2");
                    exit(1);
                }
                close(fd);
            }



            }
            execvp(cmd1[0], cmd1);
            perror("execvp first failed");
            exit(1);

        } else {
            wait(NULL);
        }
     }

}

//this function will be used to create a pipe if the command has a pipe
void pipesFunction (char **cmd1, char **cmd2){

int pipefd[2];
pid_t pid1, pid2;

if (pipe(pipefd) == -1){
    perror("pipe failed");
    exit(1);
}

// Create the first child process
 pid1 = fork();
 if (pid1 == -1){
    perror("fork failed");
    exit(1);
 } else if (pid1 == 0){
    // Child process
    dup2(pipefd[1], STDOUT_FILENO); // duplicates the write end of the pipe
    close(pipefd[0]);
   close(pipefd[1]); // closes the write end of the pipe

    // Execute the first command
    execvp(cmd1[0], cmd1);
    perror("execvp second failed");
    exit(1);
}

// Create the second child process
pid2 = fork();
if (pid2 == -1){
    perror("fork failed");
    exit(1);
} else if (pid2 == 0){
    // Child process
   close(pipefd[1]); // closes the read end of the pipe 
    dup2(pipefd[0], STDIN_FILENO); // duplicates the read end of the pipe
    close(pipefd[0]); // closes the write end of the pipe

    // Execute the second command
    execvp(cmd2[0], cmd2);
    perror("execvp third failed");
    exit(1);

}

close (pipefd[0]); // closes the read end of the pipe
close (pipefd[1]); // closes the write end of the pipe

waitpid(pid1, NULL, 0);
waitpid(pid2, NULL, 0);
}



/* Read a line from standard input and put it in a char[] */
char* readline(const char *prompt)
{
    size_t buf_len = 16;
    char *buf = xmalloc(buf_len * sizeof(char));

    printf("%s", prompt);
    if (fgets(buf, buf_len, stdin) == NULL) {
        free(buf);
        return NULL;
    }
    do {
        size_t l = strlen(buf);
        if ((l > 0) && (buf[l-1] == '\n')) {
            l--;
            buf[l] = 0;
            return buf;
        }
        if (buf_len >= (INT_MAX / 2)) memory_error();
        buf_len *= 2;
        buf = xrealloc(buf, buf_len * sizeof(char));
        if (fgets(buf + l, buf_len - l, stdin) == NULL) return buf;
    } while (1);
}

int main(void) {
    while (1) {
        struct cmdline *l;
        char *line=0;
        int i, j;
        char *prompt = "myshell>";
        char input [1000];

        while (1){
             printf("%s", prompt);
            if (fgets(input, sizeof(input), stdin) == NULL){
                break;
            }
            verification(input, l);
        }

        /* Readline use some internal memory structure that
           can not be cleaned at the end of the program. Thus
           one memory leak per command seems unavoidable yet */
        line = readline(prompt); // line is a pointer to char (string)
        if (line == 0 || ! strncmp(line,"exit", 4)) {
            terminate(line);
        }
        else {
            /* parsecmd, free line, and set it up to 0 */
            l = parsecmd( & line);

            /* If input stream closed, normal termination */
            if (l == 0) {

                terminate(0);
            }
            else if (l->err != 0) {
                /* Syntax error, read another command */
                printf("error: %s\n", l->err);
                continue;
            }
            else {
                /* there is a command to execute, let's print the sequence */
                if(l->in !=0) printf("in: %s\n", l->in);
                if(l->out != 0) printf("out: %s\n", l->out);
                printf("bg: %d\n", l->bg);

                /* Display each command of the pipe */
                for (i=0; l->seq[i]!=0; i++) {
                    char **cmd = l->seq[i];
                    printf("seq[%d]: ", i);
                    for (j=0; cmd[j]!=0; j++) {
                        printf("'%s' ", cmd[j]);
                    }
                    printf("\n");
                }

            }
        }
 
        //new code fork() and execvp()
        for (int i = 0; l->seq[i] != 0; i++) {
            char **cmd = l->seq[i];

             if (strcmp(cmd[0], "jobs") == 0){
                checkJobs();
                continue; //continue to the next iteration of the loop
             }

             if (l->bg == 1){  // checks the background of the process
                pid_t pid = fork();
                if (pid == -1){
                    perror("fork"); // error handling
                    exit(1);
                }
                else if (pid == 0){
                    redirection(l); // redirection function
                    execvp(cmd[0], cmd);
                    perror("execvp fourth failed");
                    exit(1);
                } else{
                bgJobs[jobCount].pid = pid;
                bgJobs[jobCount].status = 0; // An indication that the job is running
                bgJobs[jobCount].command = cmd[0];
                jobCount++;
                }
               
             } else {  // checks the foreground of the process

            pid_t pid = fork();
            if (pid == -1) {
                perror("fork");
                exit(1);
            }
            if (pid == 0) {
                redirection(l); // redirection function
                execvp(cmd[0], cmd);
                perror("execvp fifth failed");
                exit(1);
            }
             }
            int status;
            wait(&status);
        }
    }
    return 0;
    }
