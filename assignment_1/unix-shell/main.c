/*
    COEN 346 - Operating Systems
    Programming Assignment 1

    Thomas Mejia 40241354
    Marcelo Pedroza Hernandez 40200901

    October 7, 2024
*/

// main.c adapted from Paula Lago (2024-07-27)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h> 
#include "parser.h"
#include "utils.h"
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <fcntl.h>  
#include <signal.h>


void terminate(char *line) {
    if (line)
        free(line);  // release memory allocated to line pointer
    printf("bye\n");
    exit(0);
}


// TASK 2
// listing background processes
struct backgroundJobs{
    pid_t pid;     // checks the process id
    int status;    // checks the status of the process
    char *command; // checks the command inside of the []
};

struct backgroundJobs bgJobs[1000];  // array of background jobs

int jobCount = 0;  // counter for the number of jobs

void checkJobs() {
    for (int i = 0; i < jobCount; i++) {
        // checks if the job is running
        if (bgJobs[i].status == 0) { 
            int status = 0;
            pid_t stat = waitpid(bgJobs[i].pid, &status, WNOHANG);  // WNOHANG flag causes the process to return immediately

            if (stat == 0) {
                printf("[%d] %d Running %s\n", i + 1, bgJobs[i].pid, bgJobs[i].command);
            } else if (stat == -1 && errno != ECHILD) {
                perror("waitpid");  // there's an error while waiting for the process
            } else {
                printf("[%d] %d Done %s\n", i + 1, bgJobs[i].pid, bgJobs[i].command);
                bgJobs[i].status = 1;  // the job is finished
            }
        }
    }
}
// END TASK 1

// TASK 3 
// input and output redirection
void redirection(struct cmdline *l){
    int fd;
    if (l->in != 0){
        fd = open(l->in, O_RDONLY);  // open the file in read-only mode
        if (fd == -1){
            perror("Failed to open file (1)");
            exit(1);
        }
        // dup2() system call is used to duplicate the file descriptor
        if (dup2(fd, STDIN_FILENO) == -1){
            perror("Failed to duplicate file descriptor");
            exit(1);
        }
        close(fd);
    }
    if (l->out != 0){
        // open the file in write-only mode OR create the file if it doesn't exist OR truncate the file to zero length
        if (fd == -1){
        fd = open(l->out, O_WRONLY | O_CREAT | O_TRUNC, 0644);  
            perror("Failed to open file (2)");
            exit(1);
        }
        if (dup2(fd, STDOUT_FILENO) == -1){
            perror("Failed to duplicate file descriptor");
            exit(1);
        }
        close(fd);
    }
}
// END TASK 3


// function prototypes
void pipesFunction (char **cmd1, char **cmd2);  // TASK 4 simple pipe
void multiplePipes(struct cmdline *l);          // TASK 5 multiple pipe


// this function verifies the input
// this function makes the dream possible
void verification(char *input, struct cmdline *l) {
    // trim trailing newline if present
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    // handle 'jobs' command directly
    if (strcmp(input, "jobs") == 0) {
        checkJobs();
        return;
    }

    // create a duplicate of input for parsecmd
    // this is necessary because parsecmd modifies the input
    char *input_copy = strdup(input);

    if (!input_copy) {
        perror("Memory allocation failed (1)");
        return;
    }
    
    // first, try to parse the command using parsecmd
    l = parsecmd(&input_copy);

    if (l != NULL && l->err == NULL) {
        int num_cmds = 0;
        while (l->seq[num_cmds] != 0) {
            num_cmds++;
        }
        
        // check if we have a pipe command
        if (num_cmds >= 2) {
            multiplePipes(l);
            free(input_copy);
            return;
        }
    }
    
    // if we get here, either parsecmd failed or it's not a pipe command
    // reset for manual parsing
    free(input_copy);
    input_copy = strdup(input);
    if (!input_copy) {
        perror("Memory allocation failed (2)");
        return;
    }
    
    // initialize variables for manual parsing
    int i = 0;
    char *cmd1[1000] = {NULL};  // 1000 is an arbitrary limit
    char *saveptr;
    char *token = strtok_r(input_copy, " ", &saveptr);  // split input by space
    char *outputFile = NULL;
    char *inputFile = NULL;
    int pipeFound = 0;
    int redirectionOut = 0;
    int redirectionIn = 0;
    int consecutivePipes = 0;
    int runInBackground = 0;

    // manual parsing
    while (token != NULL) {
        if (strcmp(token, "&") == 0) {
            runInBackground = 1;  // found '&', command should run in the background
            cmd1[i] = NULL;
            break;  // end parsing after '&'
        }
        
        if (strcmp(token, "|") == 0) { // check for pipe
            consecutivePipes++;
            if (consecutivePipes > 1 || i == 0) {
                fprintf(stderr, "Invalid pipe syntax\n");
                goto cleanup;
            }
            pipeFound = 1;
            cmd1[i] = NULL;
            i = 0;
        } else {  // check for redirection
            consecutivePipes = 0;
            if (strcmp(token, ">") == 0) {
                redirectionOut = 1;  // output redirection
                token = strtok_r(NULL, " ", &saveptr);  // get next token
                if (token == NULL) { 
                    fprintf(stderr, "Missing output file\n");
                    goto cleanup;
                }
                outputFile = strdup(token);
                if (!outputFile) {
                    perror("Memory allocation failed");
                    goto cleanup;
                }
            } else if (strcmp(token, "<") == 0) {
                redirectionIn = 1;  // input redirection
                token = strtok_r(NULL, " ", &saveptr);  // get next token
                if (token == NULL) {
                    fprintf(stderr, "Missing input file\n");
                    goto cleanup;
                }
                inputFile = strdup(token);
                if (!inputFile) {
                    perror("Memory allocation failed");
                    goto cleanup;
                }
            } else {  // if all else fails, add the token to the command
                char *cmd_copy = strdup(token);
                if (!cmd_copy) {
                    perror("Memory allocation failed");
                    goto cleanup;
                }
                cmd1[i++] = cmd_copy;  // add token to command
            }
        }
        token = strtok_r(NULL, " ", &saveptr);  // get next token
    }

    // check for invalid pipe syntax
    if (pipeFound) { 
        fprintf(stderr, "Invalid pipe syntax: unsupported\n"); 
        goto cleanup;
    } else {
        if (i == 0 && !redirectionOut && !redirectionIn) {
            fprintf(stderr, "Empty command\n");
            goto cleanup;
        }
        cmd1[i] = NULL;  // terminate command

        // check for exit command
        if (cmd1[0] && strcmp(cmd1[0], "exit") == 0) {
            printf("bye\n");
            goto cleanup;
        }

        // check for cd command
        pid_t pid = fork();
        if (pid == 0) {
            if (redirectionOut && outputFile) {
                int fd = open(outputFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd == -1) {
                    perror("Failed to open output file");
                    exit(1);
                }
                if (dup2(fd, STDOUT_FILENO) == -1) {
                    perror("Failed to redirect output");
                    close(fd);
                    exit(1);
                }
                close(fd);
            }
            if (redirectionIn && inputFile) {
                int fd = open(inputFile, O_RDONLY);
                if (fd == -1) {
                    perror("Failed to open input file");
                    exit(1);
                }
                if (dup2(fd, STDIN_FILENO) == -1) {
                    perror("Failed to redirect input");
                    close(fd);
                    exit(1);
                }
                close(fd);
            }
            execvp(cmd1[0], cmd1);
            perror("execvp failed");
            exit(1);
        } else if (pid > 0) {
            if (runInBackground) {
                // store background job information
                bgJobs[jobCount].pid = pid;
                bgJobs[jobCount].status = 0; // indicates the job is running
                bgJobs[jobCount].command = strdup(input); // store the full command
                jobCount++;
                printf("[%d] %d Running %s\n", jobCount, pid, input);
            } else {
                wait(NULL);
            }
        } else {
            perror("fork failed");
        }
    }

cleanup:
    // Free all allocated memory
    free(input_copy);
    free(outputFile);
    free(inputFile);
    for (int j = 0; j < 1000; j++) {
        free(cmd1[j]);
    }
}

/*
    TASK 4 
    simple pipe implementation
*/
void pipesFunction(char **cmd1, char **cmd2) {
    // check if the commands are valid
    if (!cmd1 || !cmd2 || !cmd1[0] || !cmd2[0]) {
        fprintf(stderr, "Invalid pipe command\n");
        return;
    }

    int pipefd[2];
    pid_t pid1, pid2;

    if (pipe(pipefd) == -1) {
        perror("pipe failed");
        return;
    }

    pid1 = fork();
    if (pid1 == -1) {
        perror("fork failed");
        close(pipefd[0]);
        close(pipefd[1]);
        return;
    }

    // first child process
    if (pid1 == 0) {
        if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
            perror("dup2 failed");
            exit(1);
        }
        close(pipefd[0]);
        close(pipefd[1]);

        execvp(cmd1[0], cmd1);
        perror("execvp failed");
        exit(1);
    }

    pid2 = fork();
    if (pid2 == -1) {
        perror("fork failed");
        close(pipefd[0]);
        close(pipefd[1]);
        // kill the first child process
        kill(pid1, SIGTERM);  // signal to terminate the process
        waitpid(pid1, NULL, 0);
        return;
    }

    // second child process
    if (pid2 == 0) {
        if (dup2(pipefd[0], STDIN_FILENO) == -1) {
            perror("dup2 failed");
            exit(1);
        }
        close(pipefd[1]);
        close(pipefd[0]);

        execvp(cmd2[0], cmd2);
        perror("execvp failed");
        exit(1);
    }

    // close the pipe file descriptors in the parent process
    close(pipefd[0]);
    close(pipefd[1]);

    // wait for both children to complete
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
}


/*
    TASK 5 
    multiple pipes implementation
*/
void multiplePipes(struct cmdline *l) {
    // count the number of commands
    int num_cmds = 0;
    while (l->seq[num_cmds] != 0) {
        num_cmds++;
    }

    if (num_cmds < 2) {
        fprintf(stderr, "multiplePipes requires at least two commands.\n");
        return;
    }

    int pipefds[2 * (num_cmds - 1)]; // create pipes for each stage

    // create all pipes
    for (int i = 0; i < num_cmds - 1; i++) {
        if (pipe(pipefds + i * 2) == -1) {
            perror("pipe failed");
            exit(1);
        }
    }

    // fork a child process for each command
    int j = 0;
    for (int i = 0; i < num_cmds; i++) {
        pid_t pid = fork();

        if (pid == -1) {
            perror("fork failed");
            exit(1);
        } else if (pid == 0) {
            // handle input redirection for the first command
            if (i == 0 && l->in != NULL) {
                int fd = open(l->in, O_RDONLY);
                if (fd == -1) {
                    perror("Failed to open input file");
                    exit(1);
                }
                if (dup2(fd, STDIN_FILENO) == -1) {
                    perror("dup2 failed");
                    exit(1);
                }
                close(fd);
            }

            // handle output redirection for the last command
            if (i == num_cmds - 1 && l->out != NULL) {
                int fd = open(l->out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd == -1) {
                    perror("Failed to open output file");
                    exit(1);
                }
                if (dup2(fd, STDOUT_FILENO) == -1) {
                    perror("dup2 failed");
                    exit(1);
                }
                close(fd);
            }

            // set up pipes
            if (i > 0) {  // not the first command
                if (dup2(pipefds[(i - 1) * 2], STDIN_FILENO) == -1) {
                    perror("dup2 failed");
                    exit(1);
                }
            }

            if (i < num_cmds - 1) {  // not the last command
                if (dup2(pipefds[i * 2 + 1], STDOUT_FILENO) == -1) {
                    perror("dup2 failed");
                    exit(1);
                }
            }

            // close all pipe file descriptors in the child process
            for (int k = 0; k < 2 * (num_cmds - 1); k++) {
                close(pipefds[k]);
            }

            // execute the command!
            execvp(l->seq[i][0], l->seq[i]);
            perror("execvp failed");
            exit(1);
        }

        j += 2;  // increment by 2 to skip the pipe file descriptors in the parent process
    }

    // close all pipe file descriptors in the parent process
    for (int i = 0; i < 2 * (num_cmds - 1); i++) {
        close(pipefds[i]);
    }

    // wait for all child processes
    for (int i = 0; i < num_cmds; i++) {
        wait(NULL);
    }
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


// the one and only, the main function
int main(void) {
    while (1) {
        struct cmdline *l;
        char *line=0;
        int i, j;
        char *prompt = "myshell>";  // arbitrary prompt
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
                if (l->in != 0)
                    printf("in: %s\n", l->in);
                if (l->out != 0)
                    printf("out: %s\n", l->out);
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
 

        // fresh code fork() and execvp() for each command sequence
        for (int i = 0; l->seq[i] != 0; i++) {
            char **cmd = l->seq[i];

            if (strcmp(cmd[0], "jobs") == 0){
                checkJobs();
                continue;  // continue to the next iteration of the loop
             }

             if (l->bg == 1){  // checks the background of the process
                pid_t pid = fork();
                if (pid == -1){
                    perror("error in fork");  // error handling
                    exit(1);
                }
                else if (pid == 0){
                    redirection(l);  // redirection function
                    execvp(cmd[0], cmd);
                    perror("execvp fourth failed");
                    exit(1);
                } else{
                bgJobs[jobCount].pid = pid;
                bgJobs[jobCount].status = 0;  // indicates the job is running
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
