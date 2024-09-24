#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h> //for INT_MAX
#include "parser.h"
#include "utils.h"
#include <unistd.h>
#include <sys/types.h>


void terminate(char *line) {
    if (line)
        free(line); //release memory allocated to line pointer
    printf("bye\n");
    exit(0);
}

// QUESTION 2
struct backgroundJobs{
    pid_t pid; //checks the process id
    int status; // checks the status of the process
    char *command; // checks the command inside of the []
}

struct backgroundJobs bgJobs[100]; //array of background jobs
int jobCount = 0; //counter for the number of jobs

void checkJobs(){
    for(int i = 0; i < jobCount; i++){
        if (bgJobs[i].status == 0){ // checks if the 
           
           int status = 0;
           pid_t stat = waitpid(bgJobs[i].pid, &status, WNOHANG);

              if (stat == 0){
                printf("[%d] %s\n", bgJobs[i].pid, bgJobs[i].command);
              }
              else{
                bgJobs[i].status = 1;
              }



        }



    }

}

// END QUESTION 2

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
            pid_t pid = fork();
            if (pid == -1) {
                perror("fork");
                exit(1);
            }
            if (pid == 0) {
                execvp(cmd[0], cmd);
                perror("execvp");
                exit(1);
            }
            int status;
            wait(&status);
        }


        //Checking the background of jobs


    }
}
